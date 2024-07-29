#include "GUI.h"


GUI::GUI(int argc, char* argv[])
{
    int WIDTH = argc == 3 ? stoi(argv[1]) : 1000;
    int HEIGHT = argc == 3 ? stoi(argv[2]) : 1000;

    
    lexer = new Lexer();
    parser = new Parser;
    interpreter = new Interpreter;
    clock = 0;
    show_cursor = true;
    TTF_Init();
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Piano", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("Fonts/CascadiaMono.ttf", font_size);
    boldfont = TTF_OpenFont("Fonts/CASTELAR.ttf", font_size);
    SDL_StartTextInput();
    cursor_rect.w = 5;
    cursor_rect.h = font_size;
}

GUI::~GUI()
{
    delete lexer, parser, interpreter;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_StopTextInput();
    TTF_CloseFont(font);
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
            lexer->buffer->type(event.text.text);
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_BACKSPACE:
                lexer->buffer->backspace();
                break;
            case SDLK_RETURN:
                lexer->buffer->new_line();
                break;
            case SDLK_LEFT:
                lexer->move_cursor_left();
                break;
            case SDLK_RIGHT:
                lexer->move_cursor_right();
                break;
            case SDLK_KP_ENTER:
                cout << "press" << endl;
                interpret();
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
    SDL_RenderClear(renderer);
    render_buffer();
    render_cursor();
    SDL_RenderPresent(renderer);
}

bool GUI::work_flow()
{
    handle_events();
    lexer->lex();
    render();
    return running;
}

void GUI::render_buffer()
{
    int x = 0;
    int y = 0;
    

    for (int i = 0; i < lexer->tokens.size(); i++)
    {
        Token token = lexer->tokens[i];
        if (token.type == kw.eof)
            break;
        else if (token.type == kw.newline)
        {
            x = 0;
            y++;
        }
        else if (token.type == kw.word)
        {
            string word = token.value;
            SDL_Color color = kc.white;
            if (SYNTAX_COLORS.count(word))
                color = SYNTAX_COLORS[word];

 
            for (int j = 0; j < word.size(); j++)
            {
                char glyph = word[j];
                SDL_Surface* text_surf = TTF_RenderGlyph_Solid(font, glyph, color);
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
        else if (token.type == kw.id)
        {
            char glyph = token.value[1];
            SDL_Surface* text_surf = TTF_RenderGlyph_Solid(boldfont, glyph, kc.green);
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
        else if (token.type == kw.space)
        {
            string space = token.value;

            for (int j = 0; j < space.size(); j++)
                x++;
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
        int token_pos = 0;
        int word_pos = 0;

        for (int i = 0; i < lexer->cursor_pos; i++)
        {
            Token token = lexer->tokens[token_pos];
            if (token.type == kw.newline)
            {
                x = 0;
                y++;
            }
            else
                x++;

            word_pos++;

            if (word_pos >= token.value.size() || token.type == kw.newline || token.type == kw.id)
            {
                word_pos = 0;
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
    lexer->lex();
    parser = new Parser(lexer);
    interpreter = new Interpreter(parser);
    interpreter->interpret();
}


