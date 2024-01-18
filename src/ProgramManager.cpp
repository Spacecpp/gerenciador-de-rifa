#include "ProgramManager.h"
#include "Alegrinho.h"
#include "globals.h"
#include "window_lists.h"
#include "lists.h"


ProgramManager::ProgramManager(): firstNumber(0), lastNumber(0)
{
	
	selectedNumber = NULL;
	autosave = true;
	
	if (!load_data() )
	{
		
		firstNumber = 1; 
		lastNumber = 100;
		init_numbers();
		
	}
	
	create_number_buttons();
	
	create_other_buttons();
	
	
}



void ProgramManager::main_loop()
{
	
	while (playing)
    {
		
		events_update();
		
		while (should_run_a_frame() )
		{
			
			post_events_update();
			
			run_a_frame(); // <- game routine
			
			refresh_inputs();
			
		}
		
		if (redraw && !halt_drawing)
		{
			
			draw(); // <- game routine
			swap_buffers();
			
		}
		
    }
	
	if (autosave)
	{
		
		if (save_data() )
			printf("Salvo automaticamente\n");
		else
			printf("Erro ao salvar automaticamente\n");
		
	}
	
}



void ProgramManager::run_a_frame()
{
	
	redraw = true;
	
	if (key_down[GLFW_KEY_ESCAPE])
		playing = false;
	
	if (window_close_button_pressed)
		playing = false;
	
	
	//if (key_down[GLFW_KEY_S])
	//	save_data();
	
	
	if (window.active)
		update_window(window);
	
}



void ProgramManager::reset_data()
{
	
	for (Number & current : number)
		current.unset();
	
}



void ProgramManager::init_numbers()
{
	
	number.clear();
	
	// para fins de eficiencia apenas
	const int total_numbers = lastNumber - (firstNumber - 1);
	number.reserve(total_numbers);
	
	for (int i = firstNumber; i <= lastNumber; i++)
	{
		number.push_back(i);
	}
	
}



Number * ProgramManager::get_number(int value)
{
	
	for (Number & current : number)
	{
		
		if (current.value == value)
			return &current;
		
	}
	
	printf("Missingno. %d\n", value);
	return NULL;
	
}


/*
void ProgramManager::take_number(int value)
{
	
	for (Number & current : number)
	{
		if (current.value == value)
		{
			
			//current.taken = !current.taken;
			
			
			// atualiza na GUI
			for (GUI_Button & current_button : window.Button)
			{
				if (current_button.active && current_button.code == BUTTON::SELECT_NUMBER && current_button.secondary_code == value)
				{
					current_button.color = current.taken ? TAKEN_BUTTON_COLOR : FREE_BUTTON_COLOR;
					break;
				}
			}
			
			break;
			
		}
	}
	
}*/



void ProgramManager::draw()
{
	
	redraw = false;
	clear_screen_to_color(0.0f, 0.0f, 0.0f, 1.0f);
	
	spriteShader.use();
	glm::mat4 projection = get_default_2D_projection_matrix();
	glm::mat4 view(1.0f);
	spriteShader.setMat4("projview", projection * view);
	
	
	// fundo
	spriteCache.draw_vertical_gradient_sprite_region(whiteSquare, SCREEN_COLOR_TOP, SCREEN_COLOR_BOTTOM, 0.0f, 0.0f, whiteSquare.get_width(), whiteSquare.get_height(), 0.0f, 0.0f, screen_w, screen_h);
	
	if (window.active)
		window.draw();
	
	spriteCache.flush();
	textCache.flush();
	
}



void ProgramManager::create_number_buttons()
{
	
	const float width = screen_w;
	const float height = screen_h;
	
	window.init(0.0f, 0.0f, width, height, "", 0, false, NULL, NULL);
	window.invisible_body = true;
	//window.body_color = ColorRGBA(1.0f, 0.5f, 0.5f, 1.0f);
	
	
	// cria botoes
	const float border = 20.0f;
	const float available_space = height - border * 2.0f;
	
	int side_count = round_up( sqrt( number.size() ) );
	
	const float square_width = (available_space / (float)side_count);
	//printf("%.2f\n", available_space);
	
	float cursor_x = border;
	float cursor_y = border;
	
	const float text_scale = UIFont.get_scale_for_height(square_width * 0.7f);
	
	int count = 0;
	for (Number & current : number)
	{
		
		ColorRGBA tint = current.taken ? TAKEN_BUTTON_COLOR : FREE_BUTTON_COLOR;
		
		std::string text = std::to_string(current.value);
		if (GUI_Button * button = window.create_button( GUI_Button(text.c_str(), BUTTON::SELECT_NUMBER, current.value, cursor_x + 1.0f, cursor_y + 1.0f, square_width - 2.0f, square_width - 2.0f, tint), WINDOW_ALIGN_LEFT) )
		{
			button->text_scale = text_scale;
			button->text_color = NUMBER_TEXT_COLOR;
		}
		
		// avanca
		count++;
		if (count >= side_count)
		{
			
			cursor_x = border;
			cursor_y += square_width;
			count = 0;
			
		}
		else
		{
			cursor_x += square_width;
		}
		
	}
	
	//printf("buttons %d\n", window.Button.size() );
	
}



void ProgramManager::create_other_buttons()
{
	
	const float x = window.height + 60.0f; //window.width - 400.0f;
	const float y = 250.0f; //window.height - 100.0f;
	
	window.create_button( GUI_Button("Salvar", BUTTON::SAVE, 0, x, y, 80.0f, 30.0f, WHITE), WINDOW_ALIGN_LEFT);
	
	window.create_checkbox( GUI_CheckBox("Salvar automaticamente ao sair", &autosave, x, y + 40.0f, 16, false, 1.0f) );
	
}



void ProgramManager::update_window(GUI_Window & window)
{
	
	int result = window.update();
	if (result > 0)
	{
		
		if (window.last_clicked_button)
		{
			
			const int secondary_code = window.last_button_secondary_code;
			
			switch (result)
			{
				
				case BUTTON::EXIT:
				{
					playing = false;
					break;
				}
				
				case BUTTON::SAVE:
				{
					if (!save_data() )
						show_native_messagebox("Erro", "Erro ao salvar", MESSAGEBOX_ERROR);
					break;
				}
				
				case BUTTON::SELECT_NUMBER:
				{
					
					//take_number(secondary_code);
					selectedNumber = get_number(secondary_code);
					if (selectedNumber)
					{
						create_edit_interface();
					}
					
					break;
					
				}
				
				case BUTTON::PICK_NUMBER:
				{
					
					if (selectedNumber)
					{
						if (GUI_TextBox * target = window.get_text_box_by_code(0) )
						{
							
							std::string name = target->text;
							
							if (!name.empty() )
							{
								
								selectedNumber->set(name);
								update_button_colors();
								remove_edit_interface();
								
							}
							
						}
					}
					
					
					break;
					
				}
				
				case BUTTON::REMOVE_NUMBER:
				{
					
					if (selectedNumber)
					{
						
						if (selectedNumber->taken)
						{
							
							selectedNumber->unset();
							update_button_colors();
							remove_edit_interface();
							
						}
						
					}
					
					break;
					
				}
				
				
				
				default: printf("Unhandled button code\n");
				
			}
			
		}
		
	}
	
}



void ProgramManager::remove_edit_interface()
{
	
	window.label.clear();
	window.textBox.clear();
	
	if (GUI_Button * target = window.get_button_by_code(BUTTON::PICK_NUMBER) )
		target->destroy();
	
	if (GUI_Button * target = window.get_button_by_code(BUTTON::REMOVE_NUMBER) )
		target->destroy();
	
}



void ProgramManager::create_edit_interface()
{
	
	remove_edit_interface();
	
	const float base_x = window.height + 60.0f; //window.width - 440.0f;
	const float base_y = 100.0f;
	
	float cursor_y = base_y;
	
	
	int value = selectedNumber ? selectedNumber->value : -1;
	Number * current_number = get_number(value);
	
	const float title_scale = UIFont.get_scale_for_height(32.0f); // 20.0f
	
	std::string titulo = "Numero " + std::to_string(value);
	if (current_number && !current_number->taken)
		titulo += " (livre)";
	
	window.create_label( GUI_Label(base_x, cursor_y, titulo.c_str(), UIFont, BLACK, TEXT_ALIGN_LEFT, title_scale) );
	cursor_y += 40.0f;
	
	if (GUI_TextBox * textBox = window.create_text_box( GUI_TextBox(base_x, cursor_y, 400.0f, 32.0f, 0, "Nome", 0, BLACK, TEXTBOX_BG_COLOR, 1.0f) ) )
	{
		
		if (current_number && current_number->taken)
		{
			textBox->set_text(current_number->name);
		}
		
	}
	cursor_y += 60.0f;
	
	const float button_width = 80.0f;
	const float button_height = 30.0f;
	
	window.create_button( GUI_Button("Aplicar", BUTTON::PICK_NUMBER, 0, base_x, cursor_y, button_width, button_height, WHITE), WINDOW_ALIGN_LEFT);
	window.create_button( GUI_Button("Remover", BUTTON::REMOVE_NUMBER, 0, base_x + button_width + 20.0f, cursor_y, button_width, button_height, WHITE), WINDOW_ALIGN_LEFT);
	
}



void ProgramManager::update_button_colors()
{
	
	for (GUI_Button & current_button : window.button)
	{
		if (current_button.active && current_button.code == BUTTON::SELECT_NUMBER)
		{
			
			int value = current_button.secondary_code;
			if (Number * current_number = get_number(value) )
			{
				current_button.color = current_number->taken ? TAKEN_BUTTON_COLOR : FREE_BUTTON_COLOR;
			}
			else printf("Nao encontrou o numero correspondente %d\n", value);
			
		}
	}
	
}
