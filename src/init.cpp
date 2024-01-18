#include "init.h"
#include "globals.h"
#include "lists.h"
#include "Alegrinho_GUI.h"


bool init_program()
{
	
	if (!init_main_module() ) return false;
	if (!init_font_module() ) return false;
	
	set_new_window_resizeable(true);
	
	if (!create_main_window(SCREEN_W, SCREEN_H, "Gerenciador de Rifa") )
    {
		
        printf("Failed to create main window\n");
		show_native_messagebox("Error", "Failed to create main window", MESSAGEBOX_ERROR);
        shutdown_everything();
        return false;
    }
	
	// ---
	if (!load_data() )
	{
		printf("Error loading data\n");
		show_native_messagebox("Error", "Error loading data", MESSAGEBOX_ERROR);
		shutdown();
		return false;
	}
	
	spriteCache.create_sprite_cache();
	textCache.create_sprite_cache();
	
	return true;
	
}



bool load_data()
{
	
	printf("Loading data\n");
	
	// shaders
	if (!init_default_shader(spriteShader) ) return false;
	
	// misc
	if (!create_single_color_texture(whiteSquare, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 32, 32) ) return false;
	
	// font
	if (!UIFont.load("NotoSans-Medium.ttf", 32) ) return false;

	
	setup_GUI_pointers();
	
	printf("Data loaded!\n");
	
	return true;
	
}


void shutdown()
{
	
	printf("Shuting down...\n");
	
	spriteShader.destroy();
	whiteSquare.destroy();
	UIFont.destroy();
	
	spriteCache.destroy();
	textCache.destroy();
	
	shutdown_everything();
	
	printf("done\n");
	
}


void setup_GUI_pointers()
{
	
	GUI_font = &UIFont;
	GUI_draw_cache = &spriteCache;
	GUI_white_sprite = &whiteSquare;

	GUI_MEDIUM_FONT_HEIGHT = 16.0f; // 12.0f
	
}
