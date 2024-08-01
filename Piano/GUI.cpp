#include "GUI.h"


GUI::GUI(int argc, char* argv[])
{
    int WIDTH = argc == 3 ? stoi(argv[1]) : 1000;
    int HEIGHT = argc == 3 ? stoi(argv[2]) : 1000;

    
    buffer = new Buffer();
    lexer = new Lexer();
    gui_lexer = new GUI_Lexer();
    parser = new Parser;
    interpreter = new Interpreter;
    clock = 0;
    show_cursor = true;
    TTF_Init();
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Piano", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    plain_font = TTF_OpenFont("Fonts/CascadiaMono.ttf", font_size);
    SDL_StartTextInput();
    cursor_rect.w = 5;
    cursor_rect.h = font_size;
}

GUI::~GUI()
{
    delete lexer, parser, interpreter, gui_lexer;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_StopTextInput();
    TTF_CloseFont(plain_font);
    SDL_Quit();
}

void GUI::handle_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_TEXTINPUT:
            buffer->type(event.text.text);
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_BACKSPACE:
                buffer->backspace();
                break;
            case SDLK_RETURN:
                buffer->new_line();
                break;
            case SDLK_LEFT:
                buffer->move_cursor_left();
                break;
            case SDLK_RIGHT:
                buffer->move_cursor_right();
                break;
            case SDLK_KP_ENTER:
                interpret();
                break;
            default:
                break;
            }

        default:
            break;
        }
    }
}

void GUI::render()
{
    gui_lexer = new GUI_Lexer(buffer);
    SDL_RenderClear(renderer);
    render_buffer();
    render_cursor();
    SDL_RenderPresent(renderer);
}

bool GUI::work_flow()
{
    handle_events();
    render();
    return running;
}

void GUI::render_buffer()
{
    int x = 0;
    int y = 0;
    

    for (int i = 0; i < gui_lexer->tokens.size(); i++)
    {
        Token token = gui_lexer->tokens[i];
        if (token.type == kw.eof)
            break;
        if (token.type == kw.semi)
        {
            x = 0;
            y++;
        }
        else if (token.type == kw.space)
        {
            string space = token.value;

            for (int j = 0; j < space.size(); j++)
                x++;
        }
        else
        {
            string word = token.value;
            SDL_Color color = kc.white;

            if (SYNTAX_COLORS.count(token.type))
                color = SYNTAX_COLORS[token.type];
            


            for (int j = 0; j < word.size(); j++)
            {
                char glyph = word[j];
                SDL_Surface* text_surf = TTF_RenderGlyph_Solid(plain_font, glyph, color);
                SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_surf);
                SDL_Rect dest;

                dest.x = x * font_size;
                dest.y = y * font_size;
                dest.w = text_surf->w;
                dest.h = text_surf->h;
                SDL_RenderCopy(renderer, text, NULL, &dest);
                SDL_DestroyTexture(text);
                SDL_FreeSurface(text_surf);
                x++;
            }

      

        }
    }
}


void GUI::render_cursor()
{
    if (clock > 20)
    {
        clock = 0;
        show_cursor = !show_cursor;
    }
    if (show_cursor)
    {

        int x = 0;
        int y = 0;
        int pos = 0;
        int token_pos = 0;
        

        while (pos < buffer->cursor_pos)
        {
            Token token = gui_lexer->tokens[token_pos];

            if (token.type == kw.eof)
                break;

            if (token.type == kw.semi)
            {
                pos++;
                token_pos++;
                x = 0;
                y++;
            }
            else
            {
                int i = 0;
                while (i < token.value.size() && pos < buffer->cursor_pos)
                {
                    pos++;
                    i++;
                    x++;
                }
                token_pos++;
            }      
        }
     
        cursor_rect.x = x * font_size;
        cursor_rect.y = y * font_size;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &cursor_rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    }
    clock++;
}

void GUI::interpret()
{
    lexer = new Lexer(buffer);
    parser = new Parser(lexer);
    interpreter = new Interpreter(parser);
    interpreter->interpret();
}



