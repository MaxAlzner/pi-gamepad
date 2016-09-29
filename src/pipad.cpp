
#include "../include/pipad.h"

inline std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first) + 1);
}

namespace pipad
{
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
#if _DEBUG
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
#if _DEBUG
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
    	
        // bcm2835_init();
        // bcm2835_gpio_fsel(pins.cd4021_1_psc, BCM2835_GPIO_FSEL_OUTP);
        // bcm2835_gpio_fsel(pins.cd4021_1_clk, BCM2835_GPIO_FSEL_OUTP);
        // bcm2835_gpio_fsel(pins.cd4021_1_data, BCM2835_GPIO_FSEL_INPT);
        // bcm2835_gpio_fsel(pins.mcp3008_1_clk, BCM2835_GPIO_FSEL_OUTP);
        // bcm2835_gpio_fsel(pins.mcp3008_1_dout, BCM2835_GPIO_FSEL_INPT);
        // bcm2835_gpio_fsel(pins.mcp3008_1_din, BCM2835_GPIO_FSEL_OUTP);
        // bcm2835_gpio_fsel(pins.mcp3008_1_cs, BCM2835_GPIO_FSEL_OUTP);
        wiringPiSetup();
        pinMode(pins.cd4021_1_psc, OUTPUT);
        pinMode(pins.cd4021_1_clk, OUTPUT);
        pinMode(pins.cd4021_1_data, OUTPUT);
        pinMode(pins.mcp3008_1_clk, OUTPUT);
        pinMode(pins.mcp3008_1_dout, OUTPUT);
        pinMode(pins.mcp3008_1_din, OUTPUT);
        pinMode(pins.mcp3008_1_cs, OUTPUT);
    }
    void close()
    {
        // bcm2835_close();
    }
    
    void poll(gamepad_t& e)
    {
        // bcm2835_gpio_write(pins.cd4021_1_psc, 1);
        // bcm2835_delayMicroseconds(20);
        // bcm2835_gpio_write(pins.cd4021_1_psc, 0);
        digitalWrite(pins.cd4021_1_psc, 1);
        delayMicroseconds(20);
        digitalWrite(pins.cd4021_1_psc, 0);
        for (uint8_t i = 0; i < 8; i++)
        {
            // bcm2835_gpio_write(pins.cd4021_1_clk, 0);
            // bcm2835_delayMicroseconds(1);
            digitalWrite(pins.cd4021_1_clk, 0);
            delayMicroseconds(1);
            uint8_t btn = digitalRead(pins.cd4021_1_data);// bcm2835_gpio_lev(pins.cd4021_1_data);
            // bcm2835_gpio_write(pins.cd4021_1_clk, 1);
            digitalWrite(pins.cd4021_1_clk, 1);
#if _DEBUG
            printf("  button %d = %d\n", i, btn);
#endif
            e.buttons[i] = btn;
        }
        
#if _DEBUG
        printf("\n");
#endif
        for (uint8_t i = 0; i < 8; i++)
        {
            // bcm2835_gpio_write(pins.mcp3008_1_cs, 1);
            // bcm2835_gpio_write(pins.mcp3008_1_clk, 0);
            // bcm2835_gpio_write(pins.mcp3008_1_cs, 0);
            digitalWrite(pins.mcp3008_1_cs, 1);
            digitalWrite(pins.mcp3008_1_clk, 0);
            digitalWrite(pins.mcp3008_1_cs, 0);
            uint32_t cout = i | 0x18;
            cout <<= 3;
            for (uint8_t j = 0; j < 5; j++)
            {
                // bcm2835_gpio_write(pins.mcp3008_1_din, (cout & 0x80) ? 1 : 0);
                digitalWrite(pins.mcp3008_1_din, (cout & 0x80) ? 1 : 0);
                cout <<= 1;
                // bcm2835_gpio_write(pins.mcp3008_1_clk, 1);
                // bcm2835_gpio_write(pins.mcp3008_1_clk, 0);
                digitalWrite(pins.mcp3008_1_clk, 1);
                digitalWrite(pins.mcp3008_1_clk, 0);
            }
            
            uint16_t analog = 0;
            for (uint8_t j = 0; j < 12; j++)
            {
                // bcm2835_gpio_write(pins.mcp3008_1_clk, 1);
                // bcm2835_gpio_write(pins.mcp3008_1_clk, 0);
                digitalWrite(pins.mcp3008_1_clk, 1);
                digitalWrite(pins.mcp3008_1_clk, 0);
                analog <<= 1;
                analog |= digitalRead(pins.mcp3008_1_dout)/*bcm2835_gpio_lev(pins.mcp3008_1_dout)*/ ? 1 : 0;
            }
            
            analog >>= 1;
#if _DEBUG
            printf("  analog %d = %d\n", i, analog);
#endif
            e.analogs[i] = analog;
        }
        
        // bcm2835_gpio_write(pins.mcp3008_1_cs, 1);
        digitalWrite(pins.mcp3008_1_cs, 1);
    }
}