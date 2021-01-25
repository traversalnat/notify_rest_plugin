make -j4
sudo cp libnotify_rest.so ~/.local/lib/dde-top-panel/plugins/
pkill dde-top-panel
dde-top-panel >> /dev/null &
