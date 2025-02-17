#pragma once

#include "HFHandler.h"
#include "ETTHandler.h"

#include <cstdint>

class FrameHandler {
  private:
    size_t _ettIndex;

    size_t _hfIndex;
    size_t _hfSizeIndex;
    size_t _hfMsgidIndex;
    size_t _hfSeqIndex;
  public:
    void Init(HFHandler& hfHandler, ETTHandler& ettHandler)
    {
        hf_register_info info;

        info = { nullptr,
                    {"frame", "tll.frame",
                    FT_NONE, BASE_NONE,
                    NULL, 0x0,
                    NULL, HFILL}
                };
        _hfIndex = hfHandler.AddHF(info);

        info = { nullptr,
                    {"size", "tll.frame.size",
                    FT_UINT32, BASE_DEC,
                    NULL, 0x0,
                    NULL, HFILL}
                };
        _hfSizeIndex = hfHandler.AddHF(info);

        info = { nullptr,
                    {"msgid", "tll.frame.msgid",
                    FT_INT32, BASE_DEC,
                    NULL, 0x0,
                    NULL, HFILL}
                };
        _hfMsgidIndex = hfHandler.AddHF(info);

        info = { nullptr,
                    {"seq", "tll.frame.seq",
                    FT_INT64, BASE_DEC,
                    NULL, 0x0,
                    NULL, HFILL}
                };
        _hfSeqIndex = hfHandler.AddHF(info);

        _ettIndex = ettHandler.AddETT();
    }

    struct DissectResult {
        uint32_t size;
        int32_t msgid;
        int64_t seq;

        int offset;
    };

    DissectResult Dissect(tvbuff_t *tvb, proto_tree *mainTree, HFHandler& hfHandler, ETTHandler& ettHandler)
    {
        int offset = 0;
        proto_item *frameItem = proto_tree_add_item(mainTree, hfHandler.GetIntByIndex(_hfIndex), tvb, offset, 16, ENC_NA);
        proto_tree *frameTree = proto_item_add_subtree(frameItem, ettHandler.GetIntByIndex(_ettIndex));

        proto_tree_add_item(frameTree, hfHandler.GetIntByIndex(_hfSizeIndex), tvb, offset, 4, ENC_LITTLE_ENDIAN);
        uint32_t size = tvb_get_guint32(tvb, offset, ENC_LITTLE_ENDIAN);
        offset += 4;

        proto_tree_add_item(frameTree, hfHandler.GetIntByIndex(_hfMsgidIndex), tvb, offset, 4, ENC_LITTLE_ENDIAN);
        int32_t msgid = tvb_get_gint32(tvb, offset, ENC_LITTLE_ENDIAN);
        offset += 4;

        proto_tree_add_item(frameTree, hfHandler.GetIntByIndex(_hfSeqIndex), tvb, offset, 8, ENC_LITTLE_ENDIAN);
        int64_t seq = tvb_get_gint64(tvb, offset, ENC_LITTLE_ENDIAN);
        offset += 8;

        DissectResult result = {size, msgid, seq, offset};
        return result;
    }
};