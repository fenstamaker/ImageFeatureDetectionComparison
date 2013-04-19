all:
	g++ main.cpp gImage.cpp Comparator.cpp -I/opt/local/include -larmadillo `Magick++-config --cppflags --cxxflags --ldflags --libs` -o harris
