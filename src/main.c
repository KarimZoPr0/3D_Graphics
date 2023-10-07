#include <stdio.h>
#include "stdbool.h"
#include <SDL.h>

bool is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int window_width = 800;
int window_height = 800;

bool initialize_window( void )
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        fprintf( stderr, "Error initializing SDL \n" );
        return false;
    }

    // Use SDL to query what is the fullscreen max. width anda height
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode( 0, &display_mode );
    window_width = display_mode.w;
    window_height = display_mode.h;

    window = SDL_CreateWindow(
            NULL,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width,
            window_height,
            SDL_WINDOW_FULLSCREEN_DESKTOP
    );

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

    SDL_SetWindowFullscreen( window, SDL_WINDOW_FULLSCREEN_DESKTOP );
    return true;
}

void setup( )
{
    color_buffer = ( uint32_t * ) malloc( sizeof( uint32_t ) * window_width * window_height );

    color_buffer_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            window_width,
            window_height
    );
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

void render_color_buffer( )
{
    SDL_UpdateTexture(
            color_buffer_texture,
            NULL,
            color_buffer,
            ( int ) ( window_width * sizeof( uint32_t ) )
    );

    SDL_RenderCopy( renderer, color_buffer_texture, NULL, NULL );
}


void draw_grid( )
{
    for( int y = 0; y < window_height; y += 10 )
    {
        for( int x = 0; x < window_width; x += 10 )
        {
            if( x % 10 == 0 || y % 10 == 0 )
            {
                color_buffer[ ( y * window_width ) + x ] = 0xFF333333;
            }
        }
    }
}

void draw_rect( int x, int y, int width, int height, uint32_t color )
{
    for( int row = y; row < y + height; ++row )
    {
        for( int col = x; col < x + width; ++col )
        {
            color_buffer[ ( row * window_width ) + col ] = color;
        }
    }
}

void clear_color_buffer( uint32_t color )
{
    for( int y = 0; y < window_height; ++y )
    {
        for( int x = 0; x < window_width; ++x )
        {
            color_buffer[ ( y * window_width ) + x ] = color;
        }
    }
}

void render( )
{
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );


    draw_grid( );
    draw_rect( 1400, 900, 200, 200, 0xFFFF0000 );
    draw_rect( 1150, 500, 200, 200, 0xFF00FF00 );
    draw_rect( 900, 900, 200, 200, 0xFF0000FF );

    render_color_buffer( );
    clear_color_buffer( 0xFF000000 );

    SDL_RenderPresent( renderer );
}

void destroy_window( )
{
    free( color_buffer );
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit( );
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

    destroy_window( );
    return 0;
}