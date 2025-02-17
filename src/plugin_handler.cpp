#include <epan/packet.h>
#include "config.h"

#include "FrameHandler.h"
#include "Parser.h"
#include "Dissector.h"

#include <unordered_map>
#include <fstream>
#include <iostream>

static int gProto;

static size_t gETTProtoIndex;

static HFHandler gHFHandler;
static ETTHandler gETTHandler;
static FrameHandler frameHandler;

std::unordered_map<int32_t, ParsedMessage*> gParsedMessages;

static int
Dissect(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data _U_)
{
    col_clear(pinfo->cinfo,COL_INFO);
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "tll");

    proto_item *mainItem = proto_tree_add_item(tree, gProto, tvb, 0, -1, ENC_NA);
    proto_tree *mainTree = proto_item_add_subtree(mainItem, gETTHandler.GetIntByIndex(gETTProtoIndex));

    const auto result = frameHandler.Dissect(tvb, mainTree, gHFHandler, gETTHandler);
    int offset = result.offset;

    std::string message_name = "!unknown!";
    Dissector dissector(gHFHandler, gETTHandler);

    if (gParsedMessages.count(result.msgid) > 0) {
        dissector.DissectMessage(tvb, mainTree, gParsedMessages[result.msgid], offset);
        message_name = gParsedMessages[result.msgid]->name;
    }

    col_add_fstr(pinfo->cinfo, COL_INFO, " msgid=%d [%s] size=%d seq=%ld", result.msgid, message_name.c_str(), result.size, result.seq);

    return tvb_captured_length(tvb);
}

void RegisterProtocol()
{
    gETTProtoIndex = gETTHandler.AddETT();

    frameHandler.Init(gHFHandler, gETTHandler);
    
    tll::scheme::SchemePtr s(tll::Scheme::load(R"(yaml://./wireshark_tll_scheme.yaml)"));
    Parser parser(gHFHandler, gETTHandler);
    for (auto m = s->messages; m != nullptr; m = m->next) {
        if (gParsedMessages.count(m->msgid) > 0)
            continue;
        ParsedMessage* parsedMessage = parser.ParseMessage(m, "tll");
        gParsedMessages.insert({m->msgid, parsedMessage});
    }

    gHFHandler.LinkHF();
    gETTHandler.LinkETT();

    gProto = proto_register_protocol("TLL", "tll", "tll");

    proto_register_field_array(gProto, gHFHandler.GetData(), guint(gHFHandler.Size()));
    proto_register_subtree_array(gETTHandler.GetData(), guint(gETTHandler.Size()));
}

void HandoffPorts(const std::string& fileName, const std::string& portType, dissector_handle_t& tll_handle)
{
    std::ifstream fin;
    fin.open(fileName);

    if (!fin) {
        std::cerr << "\nERROR: Can't open file " << fileName << '\n';
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        int number;

        while (iss >> number) {
            dissector_add_uint(portType.c_str(), number, tll_handle);
        }

        if (!iss.eof()) {
            std::cerr << "\nERROR: File " << fileName << " has incorrect data\n";
        }
    }

    fin.close();
}

void HandoffProtocol()
{
    static dissector_handle_t tll_handle;

    tll_handle = create_dissector_handle(Dissect, gProto);

    HandoffPorts("wireshark_tll_udp.txt", "udp.port", tll_handle);
    HandoffPorts("wireshark_tll_tcp.txt", "tcp.port", tll_handle);
}

extern "C"
{

void proto_register_tll(void)
{
    RegisterProtocol();
}

void proto_reg_handoff_tll(void)
{
    HandoffProtocol();
}

}