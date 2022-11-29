#include "raylib.h"

//---------------------------------------------------------------------------------
// Definiciones
//----------------------------------------------------------------------------------
#define NUM_SHOOTS 50
#define NUM_MAX_ENEMIES 50
#define FIRST_WAVE 10
#define SECOND_WAVE 20
#define THIRD_WAVE 30
#define FOURTH_WAVE 40
#define FIFTH_WAVE 50

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum { FIRST = 0, SECOND, THIRD, FOURTH, FIFTH } EnemyWave;

typedef struct level1sound{
    Sound nivel1;
} level1sound;

typedef struct level2sound{
    Sound nivel2;
} level2sound;

typedef struct level3sound{
    Sound nivel3;
} level3sound;

typedef struct level4sound{
    Sound nivel4;
} level4sound;

typedef struct level5sound{
    Sound nivel5;
} level5sound;

typedef struct victoriasound{
    Sound victoria;
} victoriasound;

typedef struct Player{
    Texture2D nave;
    Rectangle rec;
    Vector2 speed;
    Color color;
} Player;

typedef struct Enemy{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Enemy;

typedef struct Shoot{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
    Sound sound;
} Shoot;

//FONDOS DEL JUEGO//
typedef struct fondo1{
    Texture2D fondo1;
} fondo1;

typedef struct fondo2{
    Texture2D fondo2;
} fondo2;

typedef struct fondo3{
    Texture2D fondo3;
} fondo3;

///
//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 1280;
static const int screenHeight = 720;

static bool gameOver = false;
static bool pause =  false;
static int score = 0;
static bool victory = false;

static Player player = { 0 };
static Enemy enemy[NUM_MAX_ENEMIES] = { 0 };
static Shoot shoot[NUM_SHOOTS] = { 0 };
static EnemyWave wave = { wave};

/// //Declarando sonidos////
static level1sound sonido1 = {sonido1};
static level2sound sonido2 = {sonido2};
static level3sound sonido3={sonido3};
static level4sound sonido4={sonido4};
static level5sound sonido5={sonido5};
static victoriasound victoria={victoria};

//Declarando fondos 
static fondo1 fondoa={fondoa};
static fondo2 fondob={fondob};
static fondo3 fondoc={fondoc};

static int shootRate = 0;
static float alpha = 0.0f;

static int activeEnemies = 0;
static int enemiesKill = 0;
static bool smooth = false;

//Scroll del fondo//
float scrollingBack = 0.0f;
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;



//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Nazca");
    InitAudioDevice();
    InitGame();
   //Sonido del juego//
    PlaySound(sonido1.nivel1);

    

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        scrollingBack -= 0.1f;
        scrollingMid -= 0.5f;
        scrollingFore -= 1.0f;

        // NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
        if (scrollingBack <= -fondoa.fondo1.width*2) scrollingBack = 0;
        if (scrollingMid <= -fondob.fondo2.width*2) scrollingMid = 0;
        if (scrollingFore <= -fondoc.fondo3.width*2) scrollingFore = 0;
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    
    UnloadGame();         // Unload loaded data (textures, sounds, models...)
    CloseAudioDevice();

    CloseWindow();        // Close window and OpenGL context
    
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
   
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void)
{

    //fondos//
    fondoa.fondo1 = LoadTexture("textures/fondo.png");
    fondob.fondo2 = LoadTexture("textures/medio.png");
    fondoc.fondo3 = LoadTexture("textures/frente.png");

  

    //



    //sonidos

    sonido1.nivel1 = LoadSound("sounds/chillpenguin.wav");
    sonido2.nivel2 = LoadSound("sounds/nivel2.mp3");
    sonido3.nivel3 = LoadSound("sounds/nivel3.mp3");
    sonido4.nivel4 = LoadSound("sounds/Nivel4.mp3");
    sonido5.nivel5 = LoadSound("sounds/Nivel5.mp3");
    victoria.victoria = LoadSound("sounds/Victoria.mp3");

    // Initialize game variables
    shootRate = 0;
    pause = false;
    gameOver = false;
    victory = false;
    smooth = false;
    wave = FIRST;
    activeEnemies = FIRST_WAVE;
    enemiesKill = 0;
    score = 0;
    alpha = 0;

    // Initialize player
    player.nave = LoadTexture("textures/naranja.png");
    player.rec.x =  20;
    player.rec.y = 340;
    player.rec.width = player.nave.width;
    player.rec.height = player.nave.height;
    player.speed.x = 5;
    player.speed.y = 5; 
    player.color = WHITE;

    // Initialize enemies
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {

        enemy[i].rec.width = 10;
        enemy[i].rec.height = 10;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;
        enemy[i].active = true;
        enemy[i].color = GRAY;
    }

    // Initialize shoots
    
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].sound = LoadSound("sounds/shot.wav");
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y + player.rec.height/4;
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = BLUE;
    }

   
   

    
}

 

// Update game (one frame)
void UpdateGame(void)
{

     

    
    if (!gameOver)
    {
        
        
        if (IsKeyPressed('Z')) pause = !pause;

        if (!pause)
        {
            switch (wave)
            {
                case FIRST:
                {
                   
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = SECOND_WAVE;
                        wave = SECOND;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case SECOND:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = THIRD_WAVE;
                        wave = THIRD;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case THIRD:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = FOURTH_WAVE;
                        wave = FOURTH;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case FOURTH:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = FIFTH_WAVE;
                        wave = FIFTH;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case FIFTH:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies){
                     PlaySound(victoria.victoria);
                     victory = true;
                     
                     }
                } break;
                default: break;
            }

            // Movimiento del jugador
            if (IsKeyDown(KEY_RIGHT)) player.rec.x += player.speed.x;
            if (IsKeyDown(KEY_LEFT)) player.rec.x -= player.speed.x;
            if (IsKeyDown(KEY_UP)) player.rec.y -= player.speed.y;
            if (IsKeyDown(KEY_DOWN)) player.rec.y += player.speed.y;

            // Colisión con el enemigo
            for (int i = 0; i < activeEnemies; i++)
            {
                if (CheckCollisionRecs(player.rec, enemy[i].rec)) gameOver = true;
            }

             // Ia de los enemigos
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active)
                {
                    enemy[i].rec.x -= enemy[i].speed.x;

                    if (enemy[i].rec.x < 0)
                    {
                        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
                    }
                }
            }

            // Muro invisible
            if (player.rec.x <= 0) player.rec.x = 0;
            if (player.rec.x + player.rec.width >= screenWidth) player.rec.x = screenWidth - player.rec.width;
            if (player.rec.y <= 0) player.rec.y = 0;
            if (player.rec.y + player.rec.height >= screenHeight) player.rec.y = screenHeight - player.rec.height;

            // Disparos
            if (IsKeyDown(KEY_SPACE))
            {
                PlaySound(shoot->sound);
                shootRate += 5;


                for (int i = 0; i < NUM_SHOOTS; i++)
                {
                    if (!shoot[i].active && shootRate%20 == 0)
                    {
                        shoot[i].rec.x = player.rec.x;
                        shoot[i].rec.y = player.rec.y + player.rec.height/4;
                        shoot[i].active = true;
                        break;
                    }
                }
            }

            // Logica del disparo
            for (int i = 0; i < NUM_SHOOTS; i++)
            {
                if (shoot[i].active)
                {
                    // Movement
                    shoot[i].rec.x += shoot[i].speed.x;

                    // Colision
                    for (int j = 0; j < activeEnemies; j++)
                    {
                        if (enemy[j].active)
                        {
                            if (CheckCollisionRecs(shoot[i].rec, enemy[j].rec))
                            {
                                shoot[i].active = false;
                                enemy[j].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                                enemy[j].rec.y = GetRandomValue(0, screenHeight - enemy[j].rec.height);
                                shootRate = 0;
                                enemiesKill++;
                                score += 100;
                            }

                            if (shoot[i].rec.x + shoot[i].rec.width >= screenWidth)
                            {
                                shoot[i].active = false;
                                shootRate = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    //pantalla de game over
    {  
        if (IsKeyPressed(KEY_ENTER))
        {
            StopSound(sonido1.nivel1);
            StopSound(sonido2.nivel2);
            StopSound(sonido3.nivel3);
            StopSound(sonido4.nivel4);
            StopSound(sonido5.nivel5);
            StopSound(victoria.victoria);
            InitGame();
            PlaySound(sonido1.nivel1);
            gameOver = false;
            
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{

    
    
    BeginDrawing();

     // Draw background image twice
            // NOTE: Texture is scaled twice its size
            DrawTextureEx(fondoa.fondo1, (Vector2){ scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(fondoa.fondo1, (Vector2){ fondoa.fondo1.width*2 + scrollingBack, 20 }, 0.0f, 2.0f, WHITE);

            // Draw midground image twice
            DrawTextureEx(fondob.fondo2, (Vector2){ scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(fondob.fondo2, (Vector2){ fondob.fondo2.width*2 + scrollingMid, 20 }, 0.0f, 2.0f, WHITE);

            // Draw foreground image twice
            DrawTextureEx(fondoc.fondo3, (Vector2){ scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(fondoc.fondo3, (Vector2){ fondoc.fondo3.width*2 + scrollingFore, 70 }, 0.0f, 2.0f, WHITE);

         
            DrawText("Miñano Lavado Diego Alonso", screenWidth - 180, screenHeight - 20, 10, RAYWHITE);

        //ClearBackground(RAYWHITE);

        if (!gameOver)
        {

            //DrawRectangleRec(player.rec, player.color);
            DrawTexture(player.nave, player.rec.x,player.rec.y,player.color);
            

            if (wave == FIRST){
             DrawText("PRIMER NIVEL", screenWidth/2 - MeasureText("PRIMER NIVEL", 40)/2, screenHeight/2 - 40, 40, Fade(RED, alpha));
             PlaySound(sonido2.nivel2);
            }
            else if (wave == SECOND ){

            StopSound(sonido1.nivel1);
            DrawText("SEGUNDO NIVEL", screenWidth/2 - MeasureText("SEGUNDO NIVEL", 40)/2, screenHeight/2 - 40, 40, Fade(BLUE, alpha));
            PlaySound(sonido3.nivel3);

            }
            else if(wave == THIRD){
                StopSound(sonido2.nivel2);
             DrawText("TERCER NIVEL", screenWidth/2 - MeasureText("TERCER NIVEL", 40)/2, screenHeight/2 - 40, 40, Fade(GREEN, alpha));
             PlaySound(sonido4.nivel4);
            } 
            else if (wave == FOURTH){
            StopSound(sonido3.nivel3);
            DrawText("CUARTO NIVEL", screenWidth/2 - MeasureText("CUARTO NIVEL", 40)/2, screenHeight/2 - 40, 40, Fade(YELLOW, alpha));
            PlaySound(sonido5.nivel5);
            
            }
            else if (wave == FIFTH){
                StopSound(sonido4.nivel4);
                 DrawText("QUINTO NIVEL", screenWidth/2 - MeasureText("QUINTO NIVEL", 40)/2, screenHeight/2 - 40, 40, Fade(VIOLET, alpha));
                 
            }

            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active) DrawRectangleRec(enemy[i].rec, enemy[i].color);
            }

            for (int i = 0; i < NUM_SHOOTS; i++)
            {
                if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);
            }

            DrawText(TextFormat("%04i", score), 20, 20, 40, GRAY);

            if (victory){

                StopSound(sonido5.nivel5);
                 DrawText("GANASTE!", screenWidth/2 - MeasureText("GANASTE!", 40)/2, screenHeight/2 - 40, 40, BLACK);
                

            }

            if (pause) DrawText("PAUSA", screenWidth/2 - MeasureText("PAUSA", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }
        else DrawText("PULSA [ENTER] PARA INTENTARLO DE NUEVO", GetScreenWidth()/2 - MeasureText("PULSA [ENTER] PARA INTENTARLO DE NUEVO", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);

    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
   
   UnloadSound(shoot->sound);
   UnloadSound(sonido1.nivel1);
   UnloadSound(sonido2.nivel2);
   UnloadSound(sonido3.nivel3);
   UnloadSound(sonido4.nivel4);
   UnloadSound(sonido5.nivel5);
   UnloadSound(victoria.victoria);
   UnloadTexture(player.nave);

   UnloadTexture(fondoa.fondo1);  // Unload background texture
    UnloadTexture(fondob.fondo2);   // Unload midground texture
    UnloadTexture(fondoc.fondo3);
   
   
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();          
    
}