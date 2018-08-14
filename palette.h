/*struct AlaColor
{
    union
    {
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };
        uint8_t raw[3];
    };
    
    inline AlaColor() __attribute__((always_inline))
    {
    }
    
    // allow construction from R, G, B
    inline AlaColor( uint8_t ir, uint8_t ig, uint8_t ib)  __attribute__((always_inline))
    : r(ir), g(ig), b(ib)
    {
    }

    // allow construction from 32-bit (really 24-bit) bit 0xRRGGBB color code
    inline AlaColor( uint32_t colorcode)  __attribute__((always_inline))
    : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
    {
    }
    
    bool operator == (const AlaColor &c) const 
    {
        return(this->r == c.r and this->g == c.g and this->b == c.b);
    }
/*
    AlaColor getPixel(AlaColor maxOut)
    {
        return AlaColor(r*maxOut.r/255, g*maxOut.g/255, b*maxOut.b/255);
    }
*//*
    AlaColor sum(AlaColor color)
    {
        int r0 = min(color.r + r, 255);
        int g0 = min(color.g + g, 255);
        int b0 = min(color.b + b, 255);
        return AlaColor(r0, g0, b0);
    }

    AlaColor interpolate(AlaColor color, float x)
    {
        int r0 = x*(color.r - r) + r;
        int g0 = x*(color.g - g) + g;
        int b0 = x*(color.b - b) + b;
        return AlaColor(r0, g0, b0);
    }
    
    AlaColor scale(float k)
    {
        int r0 = min(r*k, 255);
        int g0 = min(g*k, 255);
        int b0 = min(b*k, 255);
        return AlaColor(r0, g0, b0);
    }
  
    
    /*typedef enum {
        Aqua    = 0x00FFFF,
        Black   = 0x000000,
        Blue    = 0x0000FF,
        Cyan    = 0x00FFFF,
        Gold    = 0xFFD700,
        Gray    = 0x808080,
        Green   = 0x008000,
        Lime    = 0x00FF00,
        Magenta = 0xFF00FF,
        Navy    = 0x000080,
        Olive   = 0x808000,
        Purple  = 0x800080,
        Red     = 0xFF0000,
        Teal    = 0x008080,
        White   = 0xFFFFFF,
        Yellow  = 0xFFFF00
    } ColorCodes;*//*
} ;*/

grb cInterpolate(grb c1, grb c2, uint8_t pos) {
  grb temp;
  
  temp.g = pos * (c2.g - c1.g) / 100 + c1.g;
  temp.r = pos * (c2.r - c1.r) / 100 + c1.r;
  temp.b = pos * (c2.b - c1.b) / 100 + c1.b;

  return temp;
}

grb cScale(grb c1, uint8_t pos) {
  grb temp;
  
  temp.g = min(c1.g * pos / 100, 255);
  temp.r = min(c1.r * pos / 100, 255);
  temp.b = min(c1.b * pos / 100, 255);

  return temp;
}

/*grb cSum(grb c1, grb c2) {
  grb temp;

  temp.g = min(c1.g + c2.g, 255);
  temp.r = min(c1.r + c2.r, 255);
  temp.b = min(c1.b + c2.b, 255);

  return temp;
}*/



////////////////////////////////////////////////////////////////////////////////
// Struct definitions
////////////////////////////////////////////////////////////////////////////////

struct AlaPalette
{
    int numColors;
    grb *colors;
    
    /**
    * Get the interpolated color from the palette.
    * The argument is a floating number between 0 and numColors.
    */
    grb getPalColor(float i)
    {
        int i0 = (int)i%(numColors);
        int i1 = (int)(i+1)%(numColors);
        
        // decimal part is used to interpolate between the two colors
        float t0 = i - trunc(i);
        //float t0 = i - (int)i;

        return cInterpolate(colors[i0], colors[i1], (uint8_t)(t0 * 100));
    }
    
    /*bool operator == (const AlaPalette &c) const 
    {
        if (!(this->numColors == c.numColors))
            return false;
        
        for(int i=0; i<numColors; i++)
        {
            if (!(this->colors[i] == c.colors[i]))
                return false;
        }
        return true;
    }*/
    
};


/*struct AlaSeq
{
    int animation;
    long speed;
    long duration;
    AlaPalette palette;
};*/

/*grb cNull_[] = { };
AlaPalette alaPalNull = {0, cNull_};*/

/*grb cRgb_[] = {
  {0, 0xFF, 0},
  {0xFF, 0, 0},
  {0, 0, 0xFF}
};
AlaPalette alaPalRgb = {3, cRgb_};*/

grb cRainbow_[] = {
  {0, 0xFF, 0}, {0x55, 0xAB, 0}, {0xAB, 0xAB, 0}, {0xFF, 0, 0},
  {0xAB, 0, 0x55}, {0, 0, 0xFF}, {0, 0x55, 0xAB}, {0, 0xAB, 0x55}
};
AlaPalette alaPalRainbow = {8, cRainbow_};

grb cRainbowStripe_[] = {
  {0, 0xFF, 0}, {0, 0, 0}, {0x55, 0xAB, 0}, {0, 0, 0},
  {0xAB, 0xAB, 0}, {0, 0, 0}, {0xFF, 0, 0}, {0, 0, 0},
  {0xAB, 0, 0x55}, {0, 0, 0}, {0, 0, 0xFF}, {0, 0, 0},
  {0, 0x55, 0xAB}, {0, 0, 0}, {0, 0xAB, 0x55}, {0, 0, 0}
};
AlaPalette alaPalRainbowStripe = {16, cRainbowStripe_};

grb cParty_[] = {
  {0x00, 0x55, 0xAB}, {0x00, 0x84, 0x7C}, {0x00, 0xB5, 0x4B}, {0x00, 0xE5, 0x1B},
  {0x17, 0xE8, 0x00}, {0x47, 0xB8, 0x00}, {0x77, 0xAB, 0x00}, {0xAB, 0xAB, 0x00},
  {0x55, 0xAB, 0x00}, {0x22, 0xDD, 0x00}, {0x00, 0xF2, 0x0E}, {0x00, 0xC2, 0x3E},
  {0x00, 0x8F, 0x71}, {0x00, 0x5F, 0xA1}, {0x00, 0x2F, 0xD0}, {0x07, 0x00, 0xF9}
};
AlaPalette alaPalParty = {16, cParty_};

grb cCool_[] = {
  {0x00, 0x00, 0xFF}, {0x99, 0x00, 0xDD}, {0x44, 0x44, 0x88}, {0x00, 0x99, 0xDD}
};
AlaPalette alaPalCool = {4, cCool_};

grb cFire_[] = {
  {0x00, 0x00, 0x00}, {0x00, 0x22, 0x00},
  {0x00, 0x88, 0x00}, {0x00, 0xFF, 0x00},
  {0x66, 0xFF, 0x00}, {0xCC, 0xFF, 0x00}
};
AlaPalette alaPalFire = {6, cFire_};

/*grb cHeat_[] = {
  {0x00, 0x00, 0x00}, {0x00, 0xFF, 0x00}, {0xFF, 0xFF, 0x00}, {0xFF, 0xFF, 0xCC}
};
AlaPalette alaPalHeat = {4, cHeat_};*/
