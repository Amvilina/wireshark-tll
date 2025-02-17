# wireshark-tll

Этот плагин для `wireshark` позволяет на произвольных портах `tcp` и `udp` просматривать содержимое пришедших сообщений с произвольной схемой!

## Установка и сборка

- Клонируем проект себе: `$ git clone https://github.com/Amvilina/wireshark-tll.git`
- Устанавливаем все зависимости: `$ sudo apt install ...`
  - Зависимости можно найти в `wireshark-tll/debian/control` в строке `Build-Depends: ...`
- Переходим в папку проекта: `$ cd wireshark-tll`
- Запускаем сборку: `/wireshark-tll$ dpkg-buildpackage -us -uc`
- После успешного выполнения команды у нас появляются пакеты папкой выше: `cd ..`
- Устанавливаем пакет: `$ sudo dpkg -i *.deb`

## Как запускать?

- Установить `wireshark`: `$ sudo apt install wireshark`
- `wireshark` запускается из консоли командой: `$ wireshark`
- `wireshark` нужно запускать в папке, которая содержит следующие файлы:
  - `wireshark_tll_scheme.yaml` - схема всех сообщений, которые должен обрабатывать плагин.
  - `wireshark_tll_udp.txt` - список всех портов для `udp` через пробел.
  - `wireshark_tll_tcp.txt` - список всех портов для `tcp` через пробел.
- После запуска в консоли не должно быть никаких ошибок!

## Быстрая проверка

- Переходим в папку с примером: `$ cd wireshark-tll/example`
- Запускаем `wireshark`: `/wireshark-tll/example$ wireshark`
- В консоли не должно быть ошибок!
- В программе сверху нажимаем: `Help` -> `About Wireshark` -> `Plugins`. Мы должны увидеть наш плагин в списке: `libwireshark_tll_plugin.so`
- В программе сверху нажимаем: `File` -> `Open` -> `capture.pcap`. В нём должно быть 2 сообщения, все поля которых корректно отображаются.
