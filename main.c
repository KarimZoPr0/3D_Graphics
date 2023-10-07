#include <stdio.h>
#include "stdbool.h"
#include <SDL.h>

bool is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool initialize_window( void )
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        fprintf( stderr, "Error initializing SDL \n" );
        return false;
    }

    window = SDL_CreateWindow( NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                               SDL_WINDOW_ALWAYS_ON_TOP );
    if( !window )
    {
        fprintf( stderr, "Error initializing SDL window. \n" );
        return false;
    }

    renderer = SDL_CreateRenderer( window, -1, 0 );

    if( !renderer )
    {
        fprintf( stderr, "Error initializing SDL renderer. \n" );
        return false;
    }

    return true;
}

void setup( )
{

}

void process_input( )
{
    SDL_Event event;
    SDL_PollEvent( &event );

    switch( event.type )
    {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if( event.key.keysym.sym == SDLK_ESCAPE ) is_running = false;
            break;
    }
}

void update( )
{

}

void render( )
{
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    SDL_RenderClear( renderer );

    SDL_RenderPresent( renderer );
}

int main( int argc, char *argv[] )
{
    is_running = initialize_window( );

    setup( );

    while( is_running )
    {
        process_input( );
        update( );
        render( );
    }
    return 0;
}