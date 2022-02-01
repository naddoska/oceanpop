#include "ThemeManager.hpp"

#include "utils.hpp"

ThemeManager::ThemeManager(SDL_Renderer * renderer, OptionManager * options, Theme theme) : renderer(renderer), options(options) {
    this->change_music_on_switch = this->options->getChangeMusicOnSwitch();
    this->volume = MIX_MAX_VOLUME/8*this->options->getMusicVolume();
    this->current_volume = this->volume;
    Mix_VolumeMusic(this->current_volume);

    load(theme);
}

ThemeManager::~ThemeManager() {
    Mix_HaltMusic();
    if (this->music != NULL) {
        Mix_FreeMusic(this->music);
    }
    if (this->next_music != NULL) {
        Mix_FreeMusic(this->next_music);
    }
    SDL_DestroyTexture(this->background);
}

void ThemeManager::load(Theme theme) {
    this->theme = theme;
    loadBackground(theme);
    loadMusic(theme);
}

void ThemeManager::loadBackground(Theme theme) {
    if(this->background != NULL) {
        SDL_DestroyTexture(this->background);
    }
    switch (theme) {
        case Theme::THEME1:
            this->background = createBackgroundTexture("background1.jpg");
            break;
        case Theme::THEME2:
            this->background = createBackgroundTexture("background2.jpg");
            break;
        case Theme::THEME3:
            this->background = createBackgroundTexture("background3.jpg");
            break;
        case Theme::THEME4:
            this->background = createBackgroundTexture("background4.jpg");
            break;
        default:
            this->background = createBackgroundTexture("menu.jpg");
            break;
    }
}

void ThemeManager::loadMusic(Theme theme) {
    #ifdef __PSP__
        std::string music_file_type = "ogg";
    #else
        std::string music_file_type = "mp3";
    #endif

    if (this->next_music != NULL) {
        Mix_FreeMusic(this->next_music);
    }

    switch (theme) {
        case Theme::THEME1:
            this->next_music = Mix_LoadMUS(getResourcePath("assets/music/song1." + music_file_type).c_str());
            break;
        case Theme::THEME2:
            this->next_music = Mix_LoadMUS(getResourcePath("assets/music/song2." + music_file_type).c_str());
            break;
        case Theme::THEME3:
            this->next_music = Mix_LoadMUS(getResourcePath("assets/music/song3." + music_file_type).c_str());
            break;
        case Theme::THEME4:
            this->next_music = Mix_LoadMUS(getResourcePath("assets/music/song4." + music_file_type).c_str());
            break;
        default:
            this->next_music = NULL;
            break;
    }
}

void ThemeManager::update() {
    if ((this->music == NULL && this->next_music == NULL) || this->paused || this->volume == 0) {
        return;
    }

    if (!Mix_PlayingMusic()) {
        if (this->next_music == NULL) {
            nextSong();
        }
        if (this->music != NULL) {
            Mix_FreeMusic(this->music);
        }
        this->music = std::move(this->next_music);
        this->next_music = NULL;
        Mix_PlayMusic(this->music, 0);
    }

    if(this->current_volume < this->volume) {
        this->current_volume++;
        Mix_VolumeMusic(this->current_volume);
    }
}

void ThemeManager::draw(SDL_Renderer * renderer) {
    SDL_RenderCopy(renderer, this->background, NULL, NULL);
}

Theme ThemeManager::getNextTheme() {
    Theme theme = Theme::THEME1;
    
    if ((((int) this->theme) + 1) < (int) Theme::AMOUNT) {
        theme = (Theme) (((int) this->theme) + 1);
    }
    return theme;
}

void ThemeManager::next() {
    load(getNextTheme());
    if (change_music_on_switch) {
        Mix_HaltMusic();
    }
    unpause();
}

void ThemeManager::nextSong() {
    loadMusic(getNextTheme());
    if (change_music_on_switch) {
        Mix_HaltMusic();
        this->current_volume = this->volume;
        Mix_VolumeMusic(this->current_volume);
    }
    unpause();
}

void ThemeManager::switchTheme(int theme) {
    if (theme >= (int) Theme::AMOUNT) {
        theme = theme % (((int) Theme::AMOUNT) - 1);
        if (theme == 0) {
            theme = ((int) Theme::AMOUNT) - 1;
        }
    } else if (theme < 1) {
        theme = 1;
    }

    load((Theme) theme);
}

void ThemeManager::pause() {
    if (!this->paused) {
        this->current_volume = 0;
        Mix_VolumeMusic(this->current_volume);
        this->paused = true;
    }
}

void ThemeManager::unpause() {
    if (this->paused) {
        if (this->change_music_on_switch) {
            this->current_volume = this->volume;
            Mix_VolumeMusic(this->current_volume);
        }
        this->paused = false;
    }
}


SDL_Texture * ThemeManager::createBackgroundTexture(std::string filename) {
    std::string path = getResourcePath("assets/backgrounds/" + filename);
    SDL_Surface *img = IMG_Load(path.c_str());

    if (img == nullptr) {
        panic("couldn't load image: " + std::string(IMG_GetError()));
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);

    if (texture == nullptr) {
        panic("couldn't create texture from surface: " + std::string(SDL_GetError()));
    }

    return texture;
}