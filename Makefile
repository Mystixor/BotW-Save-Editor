main: main.cpp
	g++ -o save-editor main.cpp -fpermissive

	./save-editor ~/Downloads/game_data.sav
