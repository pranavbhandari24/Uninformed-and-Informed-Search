CXX = g++

main: 
	$(CXX) find_route.cpp
	@echo "Type ./a.out <input_file name> <origin city> <destination city> to run uninformed search."
	@echo "Type ./a.out <input_file name> <origin city> <destination city> <heuristic_file name> to run informed search."
	@echo
	@echo "Type make clean  to delete the executables."

clean:
	rm a.out
