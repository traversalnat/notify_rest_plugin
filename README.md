每经过 20 分钟就提醒休息一次,然后 20s后就提醒休息结束, 三次循环后休息5分钟

Dependents:
	dde-top-panel
	mkdir -p ~/.local/lib/dde-top-panel/plugins/

How to build:
	mkdir build
	cd build
	cmake ..
	cp ../move.sh .
	sh move.sh
