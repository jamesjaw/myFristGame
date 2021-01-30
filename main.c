// [main.c]
// this template is provided for the 2D shooter game.
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>

// If defined, logs will be shown on console and written to file.
// If commented out, logs will not be shown nor be saved.
#define LOG_ENABLED

/* Constants. */
///===============================================宣告==================================================
// Frame rate (frame per second)
const int FPS = 60;
// Display (screen) width.
const int SCREEN_W = 800;
// Display (screen) height.
const int SCREEN_H = 600;
// At most 10 audios can be played at a time.
const int RESERVE_SAMPLES = 10;
// Same as:
// const int SCENE_MENU = 1;
// const int SCENE_START = 2;
enum {
    SCENE_MENU = 1,
    SCENE_START = 2,
    // [HACKATHON 3-7]
    // TODO: Declare a new scene id.
    SCENE_SETTINGS = 3,
    SCENE_FAIL = 4,
    SCENE_SUSESS = 5

};

/* Input states */

// The active scene id.
int active_scene;
// Keyboard state, whether the key is down or not.
bool key_state[ALLEGRO_KEY_MAX];
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
bool *mouse_state;
// Mouse position.
int mouse_x, mouse_y;
// TODO: More variables to store input states such as joysticks, ...

/* Variables for allegro basic routines. */

ALLEGRO_DISPLAY* game_display;
ALLEGRO_EVENT_QUEUE* game_event_queue;
ALLEGRO_TIMER* game_update_timer;

/* Shared resources*/

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
// TODO: More shared resources or data that needed to be accessed
// across different scenes.

/* Menu Scene resources*/
ALLEGRO_BITMAP* main_img_background;
// [HACKATHON 3-1]
// TODO: Declare 2 variables for storing settings images.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_settings;
ALLEGRO_BITMAP* img_settings2;
ALLEGRO_SAMPLE* main_bgm;
ALLEGRO_SAMPLE_ID main_bgm_id;

/* Start Scene resources*/
ALLEGRO_BITMAP* start_img_background;
ALLEGRO_BITMAP* start_img_plane;
ALLEGRO_BITMAP* start_img_enemy;
ALLEGRO_BITMAP* img_exp;
ALLEGRO_SAMPLE* start_bgm;
ALLEGRO_SAMPLE_ID start_bgm_id;
// [HACKATHON 2-1]
// TODO: Declare a variable to store your bullet's image.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_bullet;
ALLEGRO_BITMAP* img_R;
ALLEGRO_BITMAP* img_HP;
ALLEGRO_BITMAP* img_faile;
ALLEGRO_BITMAP* img_susess;
ALLEGRO_BITMAP* img_set;
ALLEGRO_BITMAP* img_piont;
ALLEGRO_BITMAP* img_boss;
ALLEGRO_SAMPLE* shoot_bgm;
ALLEGRO_SAMPLE_ID shoot_bgm_id;
ALLEGRO_SAMPLE* Rshoot_bgm;
ALLEGRO_SAMPLE_ID Rshoot_bgm_id;
ALLEGRO_SAMPLE* sus_bgm;
ALLEGRO_SAMPLE_ID sus_bgm_id;
ALLEGRO_SAMPLE* fail_bgm;
ALLEGRO_SAMPLE_ID fail_bgm_id;
ALLEGRO_SAMPLE* exp_bgm;
ALLEGRO_SAMPLE_ID exp_bgm_id;
ALLEGRO_SAMPLE* crash_bgm;
ALLEGRO_SAMPLE_ID crash_bgm_id;

ALLEGRO_BITMAP* img_N1;
ALLEGRO_BITMAP* img_N2;
ALLEGRO_BITMAP* img_N3;
ALLEGRO_BITMAP* img_N4;
ALLEGRO_BITMAP* img_N5;
ALLEGRO_BITMAP* img_N6;
ALLEGRO_BITMAP* img_N7;
ALLEGRO_BITMAP* img_N8;
ALLEGRO_BITMAP* img_N9;
ALLEGRO_BITMAP* img_N0;

typedef struct {
    // The center coordinate of the image.
    float x, y;
    // The width and height of the object.
    float w, h;
    // The velocity in x, y axes.
    float vx, vy;
    // Should we draw this object on the screen.
    bool hidden;
    // The pointer to the object¡¦s image.
    ALLEGRO_BITMAP* img;
} MovableObject;
void draw_movable_object(MovableObject obj);
#define MAX_ENEMY 5
#define MAX_HP 3
#define Nh 20
#define Nw 30
// [HACKATHON 2-2]
// TODO: Declare the max bullet count that will show on screen.
// You can try max 4 bullets here and see if you needed more.
// Uncomment and fill in the code below.
#define MAX_BULLET 100
MovableObject plane;
MovableObject boss;
MovableObject enemies[MAX_ENEMY];
MovableObject HP[MAX_HP];
// [HACKATHON 2-3]
// TODO: Declare an array to store bullets with size of max bullet count.
// Uncomment and fill in the code below.
MovableObject bullets[MAX_BULLET];
MovableObject explosion[MAX_BULLET];
MovableObject R;
MovableObject num0[10];
MovableObject num1[10];
// [HACKATHON 2-4]
// TODO: Set up bullet shooting cool-down variables.
// 1) Declare your shooting cool-down time as constant. (0.2f will be nice)
// 2) Declare your last shoot timestamp.
// Uncomment and fill in the code below.
const float MAX_COOLDOWN = 0.36f;
double last_shoot_timestamp;
double last_carsh_timestamp;

int score;
int o;
int oo;
int expcount[MAX_BULLET];

/* Declare function prototypes. */

// Initialize allegro5 library
void allegro5_init(void);
// Initialize variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void game_init(void);
// Process events inside the event queue using an infinity loop.
void game_start_event_loop(void);
// Run game logic such as updating the world, checking for collision,
// switching scenes and so on.
// This is called when the game should update its logic.
void game_update(void);
// Draw to display.
// This is called when the game should draw itself.
void game_draw(void);
// Release resources.
// Free the pointers we allocated.
void game_destroy(void);
// Function to change from one scene to another.
void game_change_scene(int next_scene);
// Load resized bitmap and check if failed.
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
// [HACKATHON 3-2]
// TODO: Declare a function.
// Determines whether the point (px, py) is in rect (x, y, w, h).
// Uncomment the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);

/* Event callbacks. */
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);

/* Declare function prototypes for debugging. */

// Display error message and exit the program, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);
// Log events for later debugging, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);
// Log using va_list.
void game_vlog(const char* format, va_list arg);


///==================================main===========================================================
int main(int argc, char** argv) {
    // Set random seed for better random outcome.
    srand(time(NULL));
    allegro5_init();
    game_log("Allegro5 initialized");
    game_log("Game begin");
    // Initialize game variables.
    game_init();
    game_log("Game initialized");
    // Draw the first frame.
    game_draw();
    game_log("Game start event loop");
    // This call blocks until the game is finished.
    game_start_event_loop();
    game_log("Game end");
    game_destroy();
    return 0;
}
///=======================================init======================================================
void allegro5_init(void) {
    if (!al_init())
        game_abort("failed to initialize allegro");
    // Initialize add-ons.
    if (!al_init_primitives_addon())
        game_abort("failed to initialize primitives add-on");
    if (!al_init_font_addon())
        game_abort("failed to initialize font add-on");
    if (!al_init_ttf_addon())
        game_abort("failed to initialize ttf add-on");
    if (!al_init_image_addon())
        game_abort("failed to initialize image add-on");
    if (!al_install_audio())
        game_abort("failed to initialize audio add-on");
    if (!al_init_acodec_addon())
        game_abort("failed to initialize audio codec add-on");
    if (!al_reserve_samples(RESERVE_SAMPLES))
        game_abort("failed to reserve samples");
    if (!al_install_keyboard())
        game_abort("failed to install keyboard");
    if (!al_install_mouse())
        game_abort("failed to install mouse");
    // TODO: Initialize other addons such as video, ...

    // Setup game display.
    game_display = al_create_display(SCREEN_W, SCREEN_H);
    if (!game_display)
        game_abort("failed to create display");
    al_set_window_title(game_display, "I2P(I)_2020 Final Project <student_id>");

    // Setup update timer.
    game_update_timer = al_create_timer(1.0f / FPS);
    if (!game_update_timer)
        game_abort("failed to create timer");

    // Setup event queue.
    game_event_queue = al_create_event_queue();
    if (!game_event_queue)
        game_abort("failed to create event queue");

    // Malloc mouse buttons state according to button counts.
    const unsigned m_buttons = al_get_mouse_num_buttons();
    game_log("There are total %u supported mouse buttons", m_buttons);
    // mouse_state[0] will not be used.
    mouse_state = malloc((m_buttons + 1) * sizeof(bool));
    memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));

    // Register display, timer, keyboard, mouse events to the event queue.
    al_register_event_source(game_event_queue, al_get_display_event_source(game_display));
    al_register_event_source(game_event_queue, al_get_timer_event_source(game_update_timer));
    al_register_event_source(game_event_queue, al_get_keyboard_event_source());
    al_register_event_source(game_event_queue, al_get_mouse_event_source());
    // TODO: Register other event sources such as timer, video, ...

    // Start the timer to update and draw the game.
    al_start_timer(game_update_timer);
}
///========================================game_intit================================================
void game_init(void) {
    /* Shared resources*/
    font_pirulen_32 = al_load_font("pirulen.ttf", 32, 0);
    if (!font_pirulen_32)
        game_abort("failed to load font: pirulen.ttf with size 32");

    font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
    if (!font_pirulen_24)
        game_abort("failed to load font: pirulen.ttf with size 24");

    /* Menu Scene resources*/
    main_img_background = load_bitmap_resized("redpigbg.jpg", SCREEN_W, SCREEN_H); ///背景
    img_faile = load_bitmap_resized("faile.jpg", SCREEN_W, SCREEN_H);              ///
    img_susess = load_bitmap_resized("sus.jpeg", SCREEN_W, SCREEN_H);              ///
    img_set = load_bitmap_resized("set2.png", SCREEN_W, SCREEN_H);

    shoot_bgm = al_load_sample("Gunfire.ogg");
    if (!shoot_bgm)
        game_abort("failed to load audio: Gunfire.ogg");

    Rshoot_bgm = al_load_sample("Laser2.ogg");
    if (!Rshoot_bgm)
        game_abort("failed to load audio: Laser2.ogg");

    crash_bgm = al_load_sample("crash.ogg");
    if (!crash_bgm)
        game_abort("failed to load audio: crash.ogg");

    sus_bgm = al_load_sample("songs.ogg");
    if (!sus_bgm)
        game_abort("failed to load audio: songs.ogg");

    fail_bgm = al_load_sample("songf.ogg");
    if (!fail_bgm)
        game_abort("failed to load audio: songf.ogg");

    main_bgm = al_load_sample("song41.ogg");
    if (!main_bgm)
        game_abort("failed to load audio: S31-Night Prowler.ogg");

    // [HACKATHON 3-4]
    // TODO: Load settings images.
    // Don't forget to check their return values.
    // Uncomment and fill in the code below.
    img_settings = al_load_bitmap("settings.png");
    if (!img_settings)
        game_abort("failed to load image: settings.png");
    img_settings2 = al_load_bitmap("settings2.png");
    if (!img_settings2)
        game_abort("failed to load image: settings2.png");

    /* Start Scene resources*/
    start_img_background = load_bitmap_resized("redpiggamebg.jpg", SCREEN_W, SCREEN_H);

    start_img_plane = al_load_bitmap("plane1.png");
    if (!start_img_plane)
        game_abort("failed to load image: plane.png");

    img_exp = al_load_bitmap("explosion.png");
    if (!img_exp)
        game_abort("failed to load image: explosion.png");

    start_img_enemy = al_load_bitmap("smallfighter0006.png");
    if (!start_img_enemy)
        game_abort("failed to load image: smallfighter0006.png");

    start_bgm = al_load_sample("song42.ogg");
    if (!start_bgm)
        game_abort("failed to load audio: mythica.ogg");

    exp_bgm = al_load_sample("explosion.ogg");
    if (!exp_bgm)
        game_abort("failed to load audio: explosion.ogg");

    // [HACKATHON 2-5]
    // TODO: Initialize bullets.
    // 1) Search for a bullet image online and put it in your project.
    //    You can use the image we provided.
    // 2) Load it in by 'al_load_bitmap' or 'load_bitmap_resized'.
    // 3) If you use 'al_load_bitmap', don't forget to check its return value.
    // Uncomment and fill in the code below.
    img_bullet = al_load_bitmap("image12.png");
    if (!img_bullet)
        game_abort("failed to load image: image12.png");

    img_R = al_load_bitmap("R.png");
    if (!img_R)
        game_abort("failed to load image: R.png");

    img_HP = al_load_bitmap("hearts1.png");
    if(!img_HP)
        game_abort("failed to load image: hearts1.png");

    img_piont = al_load_bitmap("piont.png");
    if(!img_piont)
        game_abort("failed to load image: piont.png");

    img_boss = al_load_bitmap("boss.png");
    if(!img_piont)
        game_abort("failed to load image: boss.png");
       ///==================
    img_N0 = load_bitmap_resized("0.png",Nw,Nh);
    if(!img_N0)
        game_abort("failed to load image: 1.png");
    img_N1 = load_bitmap_resized("1.png",Nw,Nh);
    if(!img_N1)
        game_abort("failed to load image: 2.png");
    img_N2 = load_bitmap_resized("2.png",Nw,Nh);
    if(!img_N2)
        game_abort("failed to load image: 3.png");
    img_N3 = load_bitmap_resized("3.png",Nw,Nh);
    if(!img_N3)
        game_abort("failed to load image: 4.png");
    img_N4 = load_bitmap_resized("4.png",Nw,Nh);
    if(!img_N4)
        game_abort("failed to load image: 5.png");
    img_N5 = load_bitmap_resized("5.png",Nw,Nh);
    if(!img_N5)
        game_abort("failed to load image: 5.png");
    img_N6 = load_bitmap_resized("6.png",Nw,Nh);
    if(!img_N6)
        game_abort("failed to load image: 6.png");
    img_N7 = load_bitmap_resized("7.png",Nw,Nh);
    if(!img_N7)
        game_abort("failed to load image: 7.png");
    img_N8 = load_bitmap_resized("8.png",Nw,Nh);
    if(!img_N8)
        game_abort("failed to load image: 8.png");
    img_N9 = load_bitmap_resized("9.png",Nw,Nh);
    if(!img_N9)
        game_abort("failed to load image: 9.png");
    // Change to first scene.
    game_change_scene(SCENE_MENU);
}
int bloob[MAX_ENEMY];
int bossblood=0;

///=============================================遊戲事件=============================================
void game_start_event_loop(void) {
    bool done = false;
    ALLEGRO_EVENT event;
    int redraws = 0;
    while (!done) {
        al_wait_for_event(game_event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // Event for clicking the window close button.
            game_log("Window close button clicked");
            done = true;
        } else if (event.type == ALLEGRO_EVENT_TIMER) {
            // Event for redrawing the display.
            if (event.timer.source == game_update_timer)
                // The redraw timer has ticked.
                redraws++;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Event for keyboard key down.
            game_log("Key with keycode %d down", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = true;
            on_key_down(event.keyboard.keycode);
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            // Event for keyboard key up.
            game_log("Key with keycode %d up", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Event for mouse key down.
            game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = true;
            on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // Event for mouse key up.
            game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (event.mouse.dx != 0 || event.mouse.dy != 0) {
                // Event for mouse move.
                // game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            } else if (event.mouse.dz != 0) {
                // Event for mouse scroll.
                game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
            }
        }
        // TODO: Process more events and call callbacks by adding more
        // entries inside Scene.

        // Redraw
        if (redraws > 0 && al_is_event_queue_empty(game_event_queue)) {
            // if (redraws > 1)
            //     game_log("%d frame(s) dropped", redraws - 1);
            // Update and draw the next frame.
            game_update();
            game_draw();
            redraws = 0;
        }
    }
}
///==========================================動態====================================================
void game_update(void) {
    if (active_scene == SCENE_START) {
        plane.vx = plane.vy = 0;
        if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
            plane.vy -= 1;
        if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
            plane.vy += 1;
        if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
            plane.vx -= 1;
        if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
            plane.vx += 1;
        // 0.71 is (1/sqrt(2)).
        plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
        plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);
        // [HACKATHON 1-1]
        // TODO: Limit the plane's collision box inside the frame.
        //       (x, y axes can be separated.)
        // Uncomment and fill in the code below.
        float xx = plane.w/2;
        float yy = plane.h/2;
        if (plane.x-xx< 0)
            plane.x = 0+xx;
        else if (plane.x + xx> SCREEN_W)
            plane.x = SCREEN_W-xx;
        if (plane.y-yy < 0)
            plane.y = 0+yy;
        else if (plane.y+yy > SCREEN_H)
            plane.y = SCREEN_H-yy;
        // [HACKATHON 2-7]
        // TODO: Update bullet coordinates.
        // 1) For each bullets, if it's not hidden, update x, y
        // according to vx, vy.
        // 2) If the bullet is out of the screen, hide it.
        // Uncomment and fill in the code below.

        int i;
        for (i=0;i<MAX_BULLET;i++) {
            if (bullets[i].hidden)
                continue;
            bullets[i].x += bullets[i].vx;
            bullets[i].y += bullets[i].vy;
            int flag=0;
            for(int j=0;j<MAX_ENEMY;j++){
                if((bullets[i].x+bullets[i].w/2>enemies[j].x-enemies[j].w/2
                   &&bullets[i].x+bullets[i].w/2<enemies[j].x+enemies[j].w/2)||
                   (bullets[i].x-bullets[i].w/2>enemies[j].x-enemies[j].w/2
                   &&bullets[i].x-bullets[i].w/2<enemies[j].x+enemies[j].w/2)){
                    if((bullets[i].y-(bullets[i].h/2))<(enemies[j].y+(enemies[j].h/2))&&(bullets[i].y+(bullets[i].h/2))>(enemies[j].y+(enemies[j].h/2)))
                        if(!enemies[j].hidden){
                        flag=1;
                        bloob[j]++;
                        }
                   }
            }

            if((bullets[i].x+bullets[i].w/2>boss.x-boss.w/2
                   &&bullets[i].x+bullets[i].w/2<boss.x+boss.w/2)||
                   (bullets[i].x-bullets[i].w/2>boss.x-boss.w/2
                   &&bullets[i].x-bullets[i].w/2<boss.x+boss.w/2)) {
                    if((bullets[i].y-(bullets[i].h/2))<(boss.y+(boss.h/2)))
                        if(!boss.hidden){
                        flag=1;
                        bossblood++;
                        }
                   }


            if (bullets[i].y < 0 || flag==1){
                bullets[i].hidden = true;
            }


        }
        if(R.hidden==false){
            R.x += R.vx;
            R.y += R.vy;
            for(int j=0;j<MAX_ENEMY;j++){
                if((R.x+50>enemies[j].x-enemies[j].w/2
                   &&R.x+50<enemies[j].x+enemies[j].w/2)||
                   (R.x-50>enemies[j].x-enemies[j].w/2
                   &&R.x-50<enemies[j].x+enemies[j].w/2)){
                    if((R.y-50)<(enemies[j].y+(enemies[j].h/2))&&(R.y+50)>(enemies[j].y+(enemies[j].h/2)))
                        if(!enemies[j].hidden){
                            bloob[j]++;
                        }
                   }
            }
            if (R.y < 0 ){
                R.hidden = true;
            }
        }

        for(i=0;i<MAX_ENEMY;i++){
            if(enemies[i].hidden)
                continue;

                enemies[i].x+=sin(enemies[i].y/10);

                enemies[i].y+=0.5;

            if(enemies[i].y>SCREEN_H||bloob[i]>3){
                if(bloob[i]>3){
                    score++;
                    if (!al_play_sample(exp_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &exp_bgm_id))
                        game_abort("failed to play audio (bgm)");
                    explosion[i].x = enemies[i].x;
                    explosion[i].y = enemies[i].y;
                    explosion[i].hidden = false;
                }
                enemies[i].hidden =  true;
                enemies[i].x = (800/5)*(i+1) -80;
                enemies[i].y = 10;
                bloob[i] = 0;

                //if(score<=10)
                enemies[i].hidden = false;
            }

            for(int i=0;i<MAX_BULLET;i++){
                if(explosion[i].hidden==false){
                    expcount[i]++;
                    explosion[i].y+=explosion[i].vy;
                }
                if(expcount[i]>300){
                    explosion[i].hidden = true;
                    expcount[i] = 0;
                }
            }

            switch(score){
            case 0:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[0].hidden = false;
                num1[0].hidden = false;
                break;
            case 1:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[0].hidden = false;
                num1[1].hidden = false;
                break;
            case 2:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[0].hidden = false;
                num1[2].hidden = false;
                break;
            case 3:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[0].hidden = false;
                num1[3].hidden = false;
                break;
            case 4:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[0].hidden = false;
                num1[4].hidden = false;
                break;
            case 5:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[0].hidden = false;
                num1[5].hidden = false;
                break;
            case 6:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[0].hidden = false;
                num1[6].hidden = false;
                break;
            case 7:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[0].hidden = false;
                num1[7].hidden = false;
                break;
            case 8:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[0].hidden = false;
                num1[8].hidden = false;
                break;
            case 9:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[0].hidden = false;
                num1[9].hidden = false;
                break;
            case 10:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[1].hidden = false;
                num1[0].hidden = false;
                break;
            case 11:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[1].hidden = false;
                num1[1].hidden = false;
                break;
            case 12:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[1].hidden = false;
                num1[2].hidden = false;
                break;
            case 13:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[1].hidden = false;
                num1[3].hidden = false;
                break;
            case 14:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[1].hidden = false;
                num1[4].hidden = false;
                break;
            case 15:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[1].hidden = false;
                num1[5].hidden = false;
                break;
            case 16:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[1].hidden = false;
                num1[6].hidden = false;
                break;
            case 17:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[1].hidden = false;
                num1[7].hidden = false;
                break;
            case 18:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[1].hidden = false;
                num1[8].hidden = false;
                break;
            case 19:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[1].hidden = false;
                num1[9].hidden = false;
                break;
            case 20:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[2].hidden = false;
                num1[0].hidden = false;
                break;

            default:
                for(int i=0;i<10;i++){
                    num0[i].hidden = true;
                    num1[i].hidden = true;
                }
                num0[9].hidden = false;
                num1[9].hidden = false;
                break;
            }


            if(score>=10&&boss.hidden==true){
                boss.hidden = false;
            }
            if(boss.hidden==false){
                if(boss.vx==0){
                    boss.vx = 2;
                }
                else if(boss.x>=650){
                    boss.vx = -2;
                }
                else if(boss.x<=150){
                    boss.vx = 2;
                }
                boss.x+=boss.vx;
            }
            if(bossblood>30){
                bossblood = 0;
                boss.hidden = true;
                game_change_scene(SCENE_SUSESS);
            }


            double nowc = al_get_time();
            if((plane.x+plane.w/2>enemies[i].x-enemies[i].w/2
                   &&plane.x+plane.w/2<enemies[i].x+enemies[i].w/2)||
                   (plane.x-plane.w/2>enemies[i].x-enemies[i].w/2
                   &&plane.x-plane.w/2<enemies[i].x+enemies[i].w/2)){
                    if((plane.y+(plane.h/2)>enemies[i].y-(enemies[i].h/2)&&plane.y+(plane.h/2)<enemies[i].y+(enemies[i].h/2))||(
                       plane.y-(plane.h/2)<enemies[i].y+(enemies[i].h/2)&&plane.y-(plane.h/2)>enemies[i].y-(enemies[i].h/2)))
                        if(!enemies[i].hidden){

                            if(HP[2].hidden!=true&&HP[1].hidden!=true && HP[0].hidden!=true && nowc - last_carsh_timestamp >=1){
                                HP[2].hidden=true;
                                if (!al_play_sample(crash_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &crash_bgm_id))
                                game_abort("failed to play audio (bgm)");
                            }

                            else if(HP[2].hidden==true&&HP[1].hidden!=true && HP[0].hidden!=true && nowc - last_carsh_timestamp >=1){
                                HP[1].hidden=true;
                                if (!al_play_sample(crash_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &crash_bgm_id))
                                game_abort("failed to play audio (bgm)");
                            }

                            else if(HP[2].hidden==true&&HP[1].hidden==true && HP[0].hidden!=true && nowc - last_carsh_timestamp >=1){
                                if (!al_play_sample(crash_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &crash_bgm_id))
                                game_abort("failed to play audio (bgm)");
                                game_change_scene(SCENE_FAIL);
                            }
                            last_carsh_timestamp = nowc;
                        }
                   }

        }

        // [HACKATHON 2-8]
        // TODO: Shoot if key is down and cool-down is over.
        // 1) Get the time now using 'al_get_time'.
        // 2) If Space key is down in 'key_state' and the time
        //    between now and last shoot is not less that cool
        //    down time.
        // 3) Loop through the bullet array and find one that is hidden.
        //    (This part can be optimized.)
        // 4) The bullet will be found if your array is large enough.
        // 5) Set the last shoot time to now.
        // 6) Set hidden to false (recycle the bullet) and set its x, y to the
        //    front part of your plane.
        // Uncomment and fill in the code below.
        double now = al_get_time();
        if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN) {
            for (i = 0;i<MAX_BULLET; i++) {
                if (bullets[i].hidden==true) {
                    last_shoot_timestamp = now;
                    bullets[i].hidden = false;
                    //bullets[i].x += bullets[i].vx;
                    //bullets[i].y += bullets[i].vy;
                    bullets[i].x = plane.x;
                    bullets[i].y = plane.y;
                    if (!al_play_sample(shoot_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &shoot_bgm_id))
            game_abort("failed to play audio (bgm)");
                    break;
                }
            }
        }
        ///R///
        if(key_state[ALLEGRO_KEY_R]&&R.hidden==true){
            R.hidden = false;
            R.x = plane.x;
            R.y = plane.y;
            if (!al_play_sample(Rshoot_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &Rshoot_bgm_id))
            game_abort("failed to play audio (bgm)");
        }

    }
}
///================================================draw==============================================
void game_draw(void) {


    if (active_scene == SCENE_MENU) {
        al_draw_bitmap(main_img_background, 0, 0, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "Porco Rosso");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "Press enter to start");
        // [HACKATHON 3-5]
        // TODO: Draw settings images.
        // The settings icon should be located at (x, y, w, h) =
        // (SCREEN_W - 48, 10, 38, 38).
        // Change its image according to your mouse position.
        // Uncomment and fill in the code below.
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38, 38))
            al_draw_bitmap(img_settings2, SCREEN_W - 48, 10 , 0);
        else
            al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);
    } else if (active_scene == SCENE_START) {
        int i;

        al_draw_bitmap(start_img_background, 0, 0, 0);
        al_draw_bitmap(img_piont,SCREEN_W - 100, 5, 0);
        // [HACKATHON 2-9]
        // TODO: Draw all bullets in your bullet array.
        // Uncomment and fill in the code below.
        for (i=0;i<MAX_BULLET;i++)
            draw_movable_object(bullets[i]);

        draw_movable_object(plane);
        draw_movable_object(R);
        draw_movable_object(boss);

        for(i=0;i<10;i++)
            draw_movable_object(num0[i]);
        for(i=0;i<10;i++)
            draw_movable_object(num1[i]);

        for (i = 0; i < MAX_ENEMY; i++){
            draw_movable_object(enemies[i]);
            draw_movable_object(explosion[i]);
        }


        for(i=0;i<MAX_HP;i++)
            draw_movable_object(HP[i]);
    }
    // [HACKATHON 3-9]
    // TODO: If active_scene is SCENE_SETTINGS.
    // Draw anything you want, or simply clear the display.
    else if (active_scene == SCENE_SETTINGS) {
        al_draw_bitmap(img_set, 0, 0, 0);
    }
    else if (active_scene == SCENE_FAIL) {
        al_draw_bitmap(img_faile, 0, 0, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "Game Over");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "Press enter to play again");
    }
    else if (active_scene == SCENE_SUSESS) {
        al_draw_bitmap(img_susess, 0, 0, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "You Win");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "Press enter to play again");
    }
    al_flip_display();
}
///============================================delete================================================
void game_destroy(void) {
    // Destroy everything you have created.
    // Free the memories allocated by malloc or allegro functions.
    // Destroy shared resources.
    al_destroy_font(font_pirulen_32);
    al_destroy_font(font_pirulen_24);

    /* Menu Scene resources*/
    al_destroy_bitmap(main_img_background);
    al_destroy_sample(main_bgm);

    al_destroy_sample(exp_bgm);
    // [HACKATHON 3-6]
    // TODO: Destroy the 2 settings images.
    // Uncomment and fill in the code below.
    al_destroy_bitmap(img_settings);
    al_destroy_bitmap(img_settings2);
    al_destroy_bitmap(img_HP);
    al_destroy_bitmap(img_faile);
    al_destroy_bitmap(img_susess);
    al_destroy_bitmap(img_R);
    al_destroy_bitmap(img_boss);
    al_destroy_bitmap(img_N0);
    al_destroy_bitmap(img_N1);
    al_destroy_bitmap(img_N2);
    al_destroy_bitmap(img_N3);
    al_destroy_bitmap(img_N4);
    al_destroy_bitmap(img_N5);
    al_destroy_bitmap(img_N6);
    al_destroy_bitmap(img_N7);
    al_destroy_bitmap(img_N8);
    al_destroy_bitmap(img_N9);

    /* Start Scene resources*/
    al_destroy_bitmap(start_img_background);
    al_destroy_bitmap(start_img_plane);
    al_destroy_bitmap(start_img_enemy);
    al_destroy_sample(start_bgm);
    al_destroy_sample(shoot_bgm);
    // [HACKATHON 2-10]
    // TODO: Destroy your bullet image.
    // Uncomment and fill in the code below.
    al_destroy_bitmap(img_bullet);
    al_destroy_bitmap(img_exp);


    al_destroy_timer(game_update_timer);
    al_destroy_event_queue(game_event_queue);
    al_destroy_display(game_display);
    free(mouse_state);
}
int count=0;
///===========================================場景變換================================================
void game_change_scene(int next_scene) {
    game_log("Change scene from %d to %d", active_scene, next_scene);
    // TODO: Destroy resources initialized when creating scene.
    if (active_scene == SCENE_MENU) {
        al_stop_sample(&main_bgm_id);
        game_log("stop audio (bgm)");

    } else if (active_scene == SCENE_START) {
        al_stop_sample(&start_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_SUSESS) {
        al_stop_sample(&sus_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_FAIL) {
        al_stop_sample(&fail_bgm_id);
        game_log("stop audio (bgm)");
    }
    active_scene = next_scene;
    // TODO: Allocate resources before entering scene.
    if (active_scene == SCENE_MENU) {
        if (!al_play_sample(main_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &main_bgm_id))
            game_abort("failed to play audio (bgm)");
    } else if (active_scene == SCENE_START) {
        int i;
        score = 0;
        plane.img = start_img_plane;
        plane.x = 400;
        plane.y = 500;
        plane.w = al_get_bitmap_width(plane.img);
        plane.h = al_get_bitmap_height(plane.img);
        for (i = 0; i < MAX_ENEMY; i++) {
            enemies[i].img = start_img_enemy;
            enemies[i].w = al_get_bitmap_width(start_img_enemy);
            enemies[i].h = al_get_bitmap_height(start_img_enemy);
            //if(count==0){
            enemies[i].x =200 ;
            /*count+=15;
            }
            if(count==30){
            enemies[i].x =200- count ;
            count-=15;
            }*/
            enemies[i].y = i*30;

        }
        enemies[0].x = 300;
        enemies[1].x = 300;
        enemies[2].x = 300;
        enemies[0].y = 10;
        enemies[1].y = 100;
        enemies[2].y = 190;
        // [HACKATHON 2-6]
        // TODO: Initialize bullets.
        // For each bullets in array, set their w and h to the size of
        // the image, and set their img to bullet image, hidden to true,
        // (vx, vy) to (0, -3).
        // Uncomment and fill in the code below.
        for (int i=0;i<MAX_BULLET;i++) {
            bullets[i].w = al_get_bitmap_width(img_bullet);
            bullets[i].h = al_get_bitmap_height(img_bullet);
            bullets[i].img = img_bullet;
            bullets[i].vx = 0;
            bullets[i].vy = -3;
            bullets[i].hidden = true;
        }
        for(int i=0;i<MAX_BULLET;i++){
            explosion[i].w = al_get_bitmap_width(img_exp);
            explosion[i].h = al_get_bitmap_height(img_exp);
            explosion[i].img = img_exp;
            explosion[i].vx = 0;
            explosion[i].vy = -1;
            explosion[i].hidden = true;
        }

            R.w = al_get_bitmap_width(img_R);
            R.h = al_get_bitmap_height(img_R);
            R.img = img_R;
            R.vx = 0;
            R.vy = -4;
            R.hidden = true;

            boss.w = al_get_bitmap_width(img_boss);
            boss.h = al_get_bitmap_height(img_boss);
            boss.img = img_boss;
            boss.vx = 1;
            boss.vy = 0;
            boss.x = 350;
            boss.y = 50;
            boss.hidden = true;

        for(int i=0;i<MAX_HP;i++){
            HP[i].w = al_get_bitmap_width(img_HP);
            HP[i].h = al_get_bitmap_height(img_HP);
            HP[i].img = img_HP;
            HP[i].vx = 0;
            HP[i].vy = -3;
            HP[i].hidden = false;
        }
        HP[0].x = 15;
        HP[1].x = 45;
        HP[2].x = 75;
        HP[0].y = 15;
        HP[1].y = 15;
        HP[2].y = 15;

        for(int i=0;i<10;i++){
            num0[i].w = al_get_bitmap_width(img_N0);
            num0[i].h = al_get_bitmap_height(img_N0);
            num0[i].vx = 0;
            num0[i].vy = 0;
            num0[i].x = 755;
            num0[i].y = 15;
            num0[i].hidden = true;

            num1[i].w = al_get_bitmap_width(img_N0);
            num1[i].h = al_get_bitmap_height(img_N0);
            num1[i].vx = 0;
            num1[i].vy = 0;
            num1[i].x = 775;
            num1[i].y = 15;
            num1[i].hidden = true;
        }
        num0[0].img = img_N0;
        num0[1].img = img_N1;
        num0[2].img = img_N2;
        num0[3].img = img_N3;
        num0[4].img = img_N4;
        num0[5].img = img_N5;
        num0[6].img = img_N6;
        num0[7].img = img_N7;
        num0[8].img = img_N8;
        num0[9].img = img_N9;

        num1[0].img = img_N0;
        num1[1].img = img_N1;
        num1[2].img = img_N2;
        num1[3].img = img_N3;
        num1[4].img = img_N4;
        num1[5].img = img_N5;
        num1[6].img = img_N6;
        num1[7].img = img_N7;
        num1[8].img = img_N8;
        num1[9].img = img_N9;

        if (!al_play_sample(start_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_bgm_id))
            game_abort("failed to play audio (bgm)");
    }
    else if(active_scene==SCENE_FAIL){
        if (!al_play_sample(fail_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &fail_bgm_id))
            game_abort("failed to play audio (bgm)");
    }
    else if(active_scene==SCENE_SUSESS){
        if (!al_play_sample(sus_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &sus_bgm_id))
            game_abort("failed to play audio (bgm)");
    }
}
///========================================按鍵=====================================================
void on_key_down(int keycode) {
    if (active_scene == SCENE_MENU) {
        if (keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(SCENE_START);
    }
    // [HACKATHON 3-10]
    // TODO: If active_scene is SCENE_SETTINGS and Backspace is pressed,
    // return to SCENE_MENU.
    else if (active_scene == SCENE_SETTINGS) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
            game_change_scene(SCENE_MENU);
    }
    else if(active_scene == SCENE_FAIL){
        if (keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(SCENE_MENU);
    }
    else if(active_scene == SCENE_SUSESS){
        if (keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(SCENE_MENU);
    }
}

void on_mouse_down(int btn, int x, int y) {
    // [HACKATHON 3-8]
    // TODO: When settings clicked, switch to settings scene.
    // Uncomment and fill in the code below.
    if (active_scene == SCENE_MENU) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, SCREEN_W - 48, 10, 38, 38))
                game_change_scene(SCENE_SETTINGS);
        }
    }
}

void draw_movable_object(MovableObject obj) {
    if (obj.hidden)
        return;
    al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
    ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
    if (!loaded_bmp)
        game_abort("failed to load image: %s", filename);
    ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
    ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

    if (!resized_bmp)
        game_abort("failed to create bitmap when creating resized image: %s", filename);
    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
        al_get_bitmap_width(loaded_bmp),
        al_get_bitmap_height(loaded_bmp),
        0, 0, w, h, 0);
    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);

    game_log("resized image: %s", filename);

    return resized_bmp;
}

// [HACKATHON 3-3]
// TODO: Define bool pnt_in_rect(int px, int py, int x, int y, int w, int h)
// Uncomment and fill in the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h) {
    if(px>x&&px<x+w&&py>y&&py<y+h)
        return true;
    else
        return false;
}


// +=================================================================+
// | Code below is for debugging purpose, it's fine to remove it.    |
// | Deleting the code below and removing all calls to the functions |
// | doesn't affect the game.                                        |
// +=================================================================+

void game_abort(const char* format, ...) {
    va_list arg;
    va_start(arg, format);
    game_vlog(format, arg);
    va_end(arg);
    fprintf(stderr, "error occured, exiting after 2 secs");
    // Wait 2 secs before exiting.
    al_rest(2);
    // Force exit program.
    exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
    va_list arg;
    va_start(arg, format);
    game_vlog(format, arg);
    va_end(arg);
#endif
}

void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
    static bool clear_file = true;
    // Write log to file for later debugging.
    FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
    if (pFile) {
        va_list arg2;
        va_copy(arg2, arg);
        vfprintf(pFile, format, arg2);
        va_end(arg2);
        fprintf(pFile, "\n");
        fclose(pFile);
    }
    vprintf(format, arg);
    printf("\n");
    clear_file = false;
#endif
}
