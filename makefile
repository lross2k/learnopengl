build:
	gcc src/*.c -IInclude -LLibs -lGL -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
