#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#include <vector>
#include "Number.h"
#include "Alegrinho_GUI.h"


class ProgramManager
{
	
	public:
		int firstNumber, lastNumber;
		//std::vector<Number> numberTaken;
		std::vector<Number> number;
		GUI_Window window;
		Number * selectedNumber;
		bool autosave;
		
		ProgramManager();
		
		void main_loop();
		void run_a_frame();
		
		void reset_data();
		void init_numbers();
		Number * get_number(int value);
		//void take_number(int value);
		
		void draw();
		
		
		// GUI
		void create_number_buttons();
		void create_other_buttons();
		void update_window(GUI_Window & window);
		
		void remove_edit_interface();
		void create_edit_interface();
		
		void update_button_colors();
		
		// file IO
		bool load_data();
		bool save_data();

};
#endif
