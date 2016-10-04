
#include "../include/pipad.h"

#if defined(_DEBUG)
#include <curses.h>
#endif

inline std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first) + 1);
}

namespace pipad
{
#if defined(_DEBUG)
    static WINDOW* win = 0;
#endif
    
    static struct
    {
        struct
        {
            int mcp3008_1_clk;
            int mcp3008_1_dout;
            int mcp3008_1_din;
            int mcp3008_1_cs;
            int cd4021_1_psc;
            int cd4021_1_clk;
            int cd4021_1_data;
        };
        int registry[7];
    } pins;
    
    void start()
    {
        std::string filename(std::string(getenv("HOME")) + "/.pipad/gpio.ini");
#if defined(_DEBUG)
        printf("%s\n", filename.c_str());
#endif
    	std::ifstream file(filename.c_str());
    	if (file.is_open())
    	{
    		std::string line;
    		while (std::getline(file, line))
    		{
    			if (!line.empty() && line[0] != '#')
    			{
    				size_t seperator = line.find_first_of('=');
    				std::string key = trim(line.substr(0, seperator));
    				std::string value = trim(line.substr(seperator + 1));
#if defined(_DEBUG)
    				printf("  %s = %s\n", key.c_str(), value.c_str());
#endif
    				GPIO_GAMEPAD index = GPIO_GAMEPAD_INVALID;
    				if (key == "MCP3008_1_CLK") { index = GPIO_GAMEPAD_MCP3008_1_CLK; }
    				else if (key == "MCP3008_1_DOUT") { index = GPIO_GAMEPAD_MCP3008_1_DOUT; }
    				else if (key == "MCP3008_1_DIN") { index = GPIO_GAMEPAD_MCP3008_1_DIN; }
    				else if (key == "MCP3008_1_CS") { index = GPIO_GAMEPAD_MCP3008_1_CS; }
    				else if (key == "CD4021_1_PSC") { index = GPIO_GAMEPAD_CD4021_1_PSC; }
    				else if (key == "CD4021_1_CLK") { index = GPIO_GAMEPAD_CD4021_1_CLK; }
    				else if (key == "CD4021_1_DATA") { index = GPIO_GAMEPAD_CD4021_1_DATA; }
    				
    				pins.registry[index] = atoi(value.c_str());
    			}
    		}
    	}
    	else
    	{
    		printf("Failed open GPIO preferences.\n");
    	}
    	
    	file.close();
    	
        wiringPiSetup();
        pinMode(pins.cd4021_1_psc, OUTPUT);
        pinMode(pins.cd4021_1_clk, OUTPUT);
        pinMode(pins.cd4021_1_data, INPUT);
        pinMode(pins.mcp3008_1_clk, OUTPUT);
        pinMode(pins.mcp3008_1_dout, INPUT);
        pinMode(pins.mcp3008_1_din, OUTPUT);
        pinMode(pins.mcp3008_1_cs, OUTPUT);
        
#if defined(_DEBUG)
        win = initscr();
        noecho();
        wclear(win);
#endif
    }
    void close()
    {
#if defined(_DEBUG)
        endwin();
#endif
    }
    
    void poll(gamepad_t& e)
    {
#if defined(_DEBUG)
        wmove(win, 0, 0);
        printw("%f second(s)\n", last);
        printw("\n");
#endif
        digitalWrite(pins.cd4021_1_psc, 1);
        delayMicroseconds(20);
        digitalWrite(pins.cd4021_1_psc, 0);
        for (uint8_t i = 0; i < 8; i++)
        {
            digitalWrite(pins.cd4021_1_clk, 0);
            delayMicroseconds(1);
            uint8_t btn = digitalRead(pins.cd4021_1_data);
            digitalWrite(pins.cd4021_1_clk, 1);
#if defined(_DEBUG)
            printw("  button %d = %d\n", i, btn);
#endif
            e.buttons[i] = btn;
        }
        
#if defined(_DEBUG)
        printw("\n");
#endif
        for (uint8_t i = 0; i < 8; i++)
        {
            digitalWrite(pins.mcp3008_1_cs, 1);
            digitalWrite(pins.mcp3008_1_clk, 0);
            digitalWrite(pins.mcp3008_1_cs, 0);
            uint32_t cout = i | 0x18;
            cout <<= 3;
            for (uint8_t j = 0; j < 5; j++)
            {
                digitalWrite(pins.mcp3008_1_din, (cout & 0x80) ? 1 : 0);
                cout <<= 1;
                digitalWrite(pins.mcp3008_1_clk, 1);
                digitalWrite(pins.mcp3008_1_clk, 0);
            }
            
            uint16_t analog = 0;
            for (uint8_t j = 0; j < 12; j++)
            {
                digitalWrite(pins.mcp3008_1_clk, 1);
                digitalWrite(pins.mcp3008_1_clk, 0);
                analog <<= 1;
                if (digitalRead(pins.mcp3008_1_dout))
                {
                    analog |= 0x1;
                }
            }
            
            digitalWrite(pins.mcp3008_1_cs, 1);
            analog >>= 1;
#if defined(_DEBUG)
            printwprintw("  analog %d = %d\n", i, analog);
#endif
            e.analogs[i] = analog;
        }
#if defined(_DEBUG)
        wrefresh(win);
#endif
    }
}