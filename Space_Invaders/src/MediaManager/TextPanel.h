#pragma once
#include <vector>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class TextPanel {
    private:
        // Constructor vars storage
        const sf::Font font;
        const unsigned fontSize;
        const sf::String actor;
        const std::string line;
        const std::string actor_img_p;
        const sf::Color highlight;
        sf::RenderWindow* m_window;
        sf::View* m_view;
        bool* s_key;
        bool prev_key;
        // Internal vars storage
        std::vector<sf::String> panel_p;
        size_t panel_i = 0;
        int revealed = 0;
        // Internal timer vars
        size_t tick_b = 0; 
        size_t tick_be = 0; 
        bool blink_b = false;
        size_t reveal_b = 0;
        bool line_p = false;
        // Internal drawing vars
        sf::RectangleShape dialog_box;
        sf::Text textn;
        sf::Text text_d;
        sf::Text actor_t;
        sf::RectangleShape actor_box;
        sf::RectangleShape actor_img;
        sf::RectangleShape background;
        sf::Texture actor_img_tex;
        sf::Texture background_texture;
        std::string background_p = "res/";

        // Layout parameters
        const float margin = 20;
        const float border = 5;
        const float padding = 10;
        const float line_spacing = 0.8f;
        const float line_padding = 5;
        // Timing parameters
        const float rate_be = 0.5;      //rate in Hz of ellipsis animation
        const float rate_b = 2;         //rate in Hz of next arrow blink
        const float reveal_s = 25.0f;   // reveal speed in chars/s (max. 60)
        const size_t hold_s_boost = 2;  //factor to boost reveal speed by if key held
        
        // Internal utility methods
        static std::string ToUTF8(const sf::String &original);
        sf::String text();

    public:
        TextPanel(sf::String string, const sf::String actor, const sf::Color highlight, const sf::Font &font, unsigned fontSize, sf::RenderWindow* m_window, sf::View* m_view, bool* s_key, std::string a_line = "", std::string actor_img_p = "", std::string background_p = "", bool bold = false);
        ~TextPanel();
        void loadTex();
         
        bool next();
        void tick();
        void draw();
};