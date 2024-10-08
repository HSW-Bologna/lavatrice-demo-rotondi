# Demo Lavatrice Rotondi

## Compilazione

Il progetto usa [svadilfari](https://github.com/Maldus512/svadilfari). Per compilare e' necessario:

 * Scaricare lua 5.4 e luarocks.
 * Creare una cartella `.env`
 * Eseguire `luarocks install --tree .env https://raw.githubusercontent.com/Maldus512/svadilfari/refs/heads/main/svadilfari-0.1-1.rockspec`
 * Eseguire `luarocks --tree .env path > export.sh`
 * Prima di compilare preparare il terminale esportando `export.sh`: `. export.sh`
 * Configurare la compilazione con il comando `svad.lua configure`
 * Compilare con il comando `svad.lua build`
 * Il binario finale e' `cache/lavatrice-demo`
