
OBJECTS=AABB.o Card.o CardImageLoader.o CellAABBMap.o CellCardMap.o Cell.o GameBoard.o SDL.o \
	SDL/EventHandler.o SDL/Rect.o SDL/Renderer.o SDL/Texture.o 

CC=g++
CXXFLAGS=-pipe -Wall -Wextra -Werror -Wfatal-errors -Wpedantic -Weffc++ -std=c++11 -Iinclude/ `pkg-config sdl2 SDL2_image --cflags`
DEBUG_FLAGS=-g
RELEASE_FLAGS=-s -DNDEBUG
LDLIBS=`pkg-config sdl2 SDL2_image --libs`
DIRS=build/ build/SDL/ deps/ deps/SDL/ tests/

vpath %.cpp src

freecell: build/main.o $(addprefix build/, $(OBJECTS))
	$(CC)  $(LDLIBS) $^ -o $@

$(DIRS):
	mkdir -p $(DIRS)

directories: $(DIRS)

-include deps/*.deps

build/%.o: %.cpp | directories
	@ echo " building \e[92m $* \e[0m"
	@$(CC) $(CXXFLAGS) $(RELEASE_FLAGS) -c $< -o $@
	@$(CC) $(CXXFLAGS) -MM $< -MT $@ > deps/$*.deps

#debug: | freecell_debug

#freecell_debug: build/main.o $(addprefix $(OBJECTS))
#	$(CC) $(CPPFLAGS) $(DEBUG_FLAGS) $(LDLIBS) $^ -o $@

tests: tests/01_cell_test | directories

tests/%: test_src/%.cpp
	$(CC) $(CPPFLAGS) $(DEBUG_FLAGS) $(LDLIBS) $< -o $@

clean:
	rm -rf freecell_debug freecell $(DIRS)

.PHONY: debug clean directories
