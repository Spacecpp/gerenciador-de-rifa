#include "ProgramManager.h"
#include "Alegrinho.h"



bool ProgramManager::load_data()
{
	
	FileHandler file;
	if (file.open("data.txt", false) )
	{
		
		reset_data();
		
		file.skip_UTF8_BOM();
		
		// carrega os limites
		file.read_variable(firstNumber);
		file.read_variable(lastNumber);
		
		if (firstNumber >= 0 && firstNumber <= lastNumber && lastNumber <= 1000)
		{
			
			init_numbers();
			
			int read_count = 0;
			
			bool ok = true;
			while (ok)
			{
				
				int value = 0;
				if (file.read_variable(value) )
				{
					
					std::string name = file.read_line();
					trim(name);
					
					if (!name.empty() )
					{
						
						if (Number * n = get_number(value) )
						{
							n->set(name);
							read_count++;
						}
						
					}
					else
						printf("nome vazio de numero %d\n", value);
					
				}
				else ok = false;
				
			}
			
			printf("Lidos %d entidades\n", read_count);
			
		}
		else printf("Contagem de numeros invalida: %d, %d\n", firstNumber, lastNumber);
		
		file.close();
		return true;
		
	}
	printf("Nao pode carregar o arquivo de dados\n");
	
	return false;
	
}



bool ProgramManager::save_data()
{
	
	
	FileHandler file;
	if (file.create("data.txt", false) )
	{
		
		file.write_UTF8_BOM();
		
		// limites
		file.write_variable(firstNumber);
		file.write_variable(lastNumber);
		file.write_endline();
		
		for (Number & current : number)
		{
			
			// salva somente os pegos, valor seguido de nome
			if (current.taken)
			{
				
				file.write_variable(current.value);
				file.write_text(current.name);
				file.write_endline();
				
			}
			
		}
		
		file.close();
		printf("Arquivo salvo\n");
		return true;
		
	}
	else printf("Nao pode salvar o arquivo de dados\n");
	
	return false;
	
}
