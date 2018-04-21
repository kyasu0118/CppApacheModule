//
//  GazoShori.hpp
//
//  Created by kyasu on 2018/03/27.
//  Copyright © 2018 kyasu. All rights reserved.
//
#ifndef GazoShori_hpp
#define GazoShori_hpp

#include <array>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <chrono>

namespace gs
{
    inline static void time_measurement( const std::function<void()>& func )
    {
        auto begin = std::chrono::system_clock::now();
        func();
        auto end = std::chrono::system_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "(ms)" << std::endl;
    }

    namespace core
    {
        inline static const float radian_to_degree( const float radian )
        {
            return radian * 57.2957795457f;
        }
        
        inline static const float degree_to_radian( const float degree )
        {
            return degree * 0.01745329251f;
        }

        inline static const int fast_min( const int x, const int y )
        {
            return y + ((x - y) & ((x - y) >> ((sizeof(int) << 3) - 1)));
        }
        
        inline static const int fast_max( const int x, const int y )
        {
            return x - ((x - y) & ((x - y) >> ((sizeof(int) << 3) - 1)));
        }
        
        inline static const int fast_abs( const int x )
        {
            return fast_max( x, -x );
        }
        
        inline static const int limit( const int value, const int min, const int max )
        {
            return fast_min( fast_max( value, min ), max );
        }
        
        static const int fixed_point_table[] = {0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,244,248,252,257,261,265,269,273,277,281,285,289,293,297,301,305,309,313,317,321,325,329,333,337,341,345,349,353,357,361,365,369,373,377,381,385,389,393,397,401,405,409,413,417,421,425,429,433,437,441,445,449,453,457,461,465,469,473,477,481,485,489,493,497,501,505,509,514,518,522,526,530,534,538,542,546,550,554,558,562,566,570,574,578,582,586,590,594,598,602,606,610,614,618,622,626,630,634,638,642,646,650,654,658,662,666,670,674,678,682,686,690,694,698,702,706,710,714,718,722,726,730,734,738,742,746,750,754,758,762,766,771,775,779,783,787,791,795,799,803,807,811,815,819,823,827,831,835,839,843,847,851,855,859,863,867,871,875,879,883,887,891,895,899,903,907,911,915,919,923,927,931,935,939,943,947,951,955,959,963,967,971,975,979,983,987,991,995,999,1003,1007,1011,1015,1019,1024};

        template <class T> struct Point
        {
            T x, y;

            Point(){}
            
            template <class OtherType>
            Point( OtherType x, OtherType y )
            {
                this->x = (T)x;
                this->y = (T)y;
            }

            template <class OtherType>
            operator Point<OtherType>()
            {
                return Point<OtherType>( (OtherType)x, (OtherType)y );
            }
        };
    
        template <class T> struct Size
        {
            T width, height;
            
            Size(){}

            template <class OtherType>
            Size( OtherType width, OtherType height )
            {
                this->width = (T)width;
                this->height = (T)height;
            }
            
            template <class OtherType>
            operator Size<OtherType>()
            {
                return Size<OtherType>( (OtherType)width, (OtherType)height );
            }
            
            bool operator ==( const Size& right ) const
            {
                return width == right.width && height == right.height;
            }
            bool operator !=( const Size& right ) const
            {
                return width != right.width || height != right.height;
            }
        };
    
        template <class T> struct Rectangle
        {
            union
            {
                struct
                {
                    T x, y, width, height;
                };
                struct
                {
                    Point<T> point;
                    Size<T> size;
                };
            };
            Rectangle(){}

            template <class OtherType>
            Rectangle( OtherType x, OtherType y, OtherType width, OtherType height )
            {
                this->x = (T)x;
                this->y = (T)y;
                this->width = (T)width;
                this->height = (T)height;
            }
            
            Rectangle( Point<T> point, Size<T> size )
            {
                this->point = point;
                this->size  = size;
            }
            
            template <class OtherType>
            operator Rectangle<OtherType>()
            {
                return Rectangle<OtherType>( point, size );
            }
        };

        template <class T> struct Circle
        {
            union
            {
                struct
                {
                    T x, y;
                };
                Point<T> point;
            };
            T radius;
            
            Circle(){}
            
            template <class OtherType>
            Circle( OtherType x, OtherType y, OtherType radius )
            {
                this->x = (T)x;
                this->y = (T)y;
                this->radius = (T)radius;
            }
            
            template <class OtherType>
            Circle( Point<OtherType> point, OtherType radius )
            {
                this->point  = point;
                this->radius = (T)radius;
            }
            
            template <class OtherType>
            operator Circle<OtherType>()
            {
                return Circle<OtherType>( point, (OtherType)radius );
            }
        };
    }
    
    struct Vector2
    {
        float x = 0, y = 0;
        
        Vector2(){}
        Vector2( float x, float y )
        {
            this->x = x;
            this->y = y;
        }

        //|A||B|cosine
        inline float dot(const Vector2& b) const
        {
            return x*b.x + y*b.y;
        }
        
        //|A||B|sine or parallelogram area.
        inline float cross(const Vector2& b) const
        {
            return x*b.y - y*b.x;
        }
        
        inline float magnitude() const
        {
            return std::sqrt( x*x+y*y );
        }
        
        inline float magnitude_square() const
        {
            return x*x+y*y;
        }

        inline Vector2 normalize() const
        {
            const float m = magnitude();
            return Vector2( x / m, y / m );
        }
        
        inline float angle() const
        {
            return core::radian_to_degree( std::atan2( y, x ) );
        }
        
        inline Vector2 rotate_radian( float radian ) const
        {
            const float sin_sita = std::sin( radian );
            const float cos_sita = std::cos( radian );
            
            return Vector2( x*cos_sita - y*sin_sita, x*sin_sita + y*cos_sita );
        }

        inline Vector2 rotate( float degree ) const
        {
            return rotate_radian( core::degree_to_radian( degree ) );
        }

        inline Vector2 operator+( const Vector2& right ) const
        {
            return Vector2( x+right.x, y+right.y);
        }
        
        inline Vector2 operator-( const Vector2& right ) const
        {
            return Vector2( x-right.x, y-right.y);
        }
        
        inline Vector2 operator*( const float right ) const
        {
            return Vector2( x*right, y*right);
        }
        
        inline Vector2& operator+=( const Vector2& right )
        {
            x += right.x;
            y += right.y;
            return *this;
        }
        
        inline Vector2& operator-=( const Vector2& right )
        {
            x -= right.x;
            y -= right.y;
            return *this;
        }

        inline Vector2& operator*=( const float right )
        {
            x *= right;
            y *= right;
            return *this;
        }
        
        inline float sine() const
        {
            return y / magnitude();
        }
        
        inline float normalize_sine() const
        {
            return y;
        }
        
        inline float cosine() const
        {
            return x / magnitude();
        }
        
        inline float normalize_cosine() const
        {
            return y;
        }
        
        inline float tangent() const
        {
            return y / x;
        }
        
        inline static Vector2 direction( const float degree )
        {
            const float radian = core::degree_to_radian( degree );
            return Vector2( cos( radian ), sin( radian ) );
        }
    };
    
    using PointI     = core::Point<int>;
    using PointF     = core::Point<float>;
    using SizeI      = core::Size<int>;
    using SizeF      = core::Size<float>;
    using RectangleI = core::Rectangle<int>;
    using RectangleF = core::Rectangle<float>;
    using CircleI    = core::Circle<int>;
    using CircleF    = core::Circle<float>;
    
#define ColorConcept( NAME, T, N, ... ) \
    struct NAME\
    { \
        static const int CHANNEL = N; \
        static const int TYPE_BYTE = sizeof(T); \
        union \
        { \
            T channels[N]; \
            struct \
            { \
                T __VA_ARGS__; \
            }; \
        }; \
        inline T* begin(){ return channels; } \
        inline T* end(){ return channels + CHANNEL; } \
        inline const T* cbegin() const { return channels; } \
        inline const T* cbend() const { return channels + CHANNEL; } \
        inline const T operator[](int index) const { return channels[index]; } \
        inline T& operator[](int index) { return channels[index]; } \
    }
    ColorConcept( GRAY,   uint8_t, 1, L          );
    ColorConcept( RGB,    uint8_t, 3, R, G, B    );
    ColorConcept( RGBA,   uint8_t, 4, R, G, B, A );
    ColorConcept( GRAY_F, float, 1, L          );
    
    /*
       H = hue. color degree angle. 
           R =    0 degree.
           G =  120 degree.
           B = -120 degree.
       
       M = magnitude. strength of color.
     
       B = base luminance.
     */
    ColorConcept( HMB,  float, 3, H, M, B );
#undef ColorConcept

    // Unimplemented
    struct ColorBufferGRAY
    {
        int L = 0;
    };

    // Unimplemented
    struct ColorBufferGRAY_F
    {
        float L = 0;
    
        ColorBufferGRAY_F(){}
        ColorBufferGRAY_F( const float value )
        {
            L = value;
        }
        ColorBufferGRAY_F( const GRAY_F value )
        {
            *this = value;
        }
        
        inline ColorBufferGRAY_F& operator=( const GRAY_F value )
        {
            L = value.L;
            return *this;
        }
        
        inline ColorBufferGRAY_F& operator=( const ColorBufferGRAY_F value )
        {
            L = value.L;
            return *this;
        }

        inline ColorBufferGRAY_F operator*( const float value ) const
        {
            return { L * value };
        }

        inline ColorBufferGRAY_F operator*( const GRAY_F value ) const
        {
            return { L * value.L };
        }

        inline ColorBufferGRAY_F operator*( const ColorBufferGRAY_F value ) const
        {
            return { L * value.L };
        }
        
        inline ColorBufferGRAY_F operator+( const float value ) const
        {
            return {L + value};
        }

        inline ColorBufferGRAY_F operator+( const GRAY_F value ) const
        {
            return {L + value.L};
        }
        
        inline ColorBufferGRAY_F& operator+=( const float value )
        {
            L += value;
            return *this;
        }

        inline ColorBufferGRAY_F& operator+=( const GRAY_F value )
        {
            L += value.L;
            return *this;
        }

        inline ColorBufferGRAY_F& operator+=( const ColorBufferGRAY_F value )
        {
            L += value.L;
            return *this;
        }
        
        inline ColorBufferGRAY_F operator>>( const int value ) const
        {
            return { L / (float)(1<<value) };
        }
        
        inline operator GRAY_F() const
        {
            return { L };
        }
    };

    struct ColorBufferRGB
    {
        int R = 0;
        int G = 0;
        int B = 0;
        
        ColorBufferRGB(){}

        ColorBufferRGB( const int value )
        {
            this->R = value;
            this->G = value;
            this->B = value;
        }

        ColorBufferRGB( const int R, const int G, const int B )
        {
            this->R = R;
            this->G = G;
            this->B = B;
        }

        ColorBufferRGB( const RGB value )
        {
            *this = value;
        }
        
        inline ColorBufferRGB& operator=( const RGB value )
        {
            R = (int)value.R;
            G = (int)value.G;
            B = (int)value.B;
            
            return *this;
        }
        
        inline ColorBufferRGB operator+( const int value ) const
        {
            return { R+value, G+value, B+value };
        }
        
        inline ColorBufferRGB& operator+=( const int value )
        {
            R += value;
            G += value;
            B += value;
            
            return *this;
        }
        
        inline ColorBufferRGB operator+( const ColorBufferRGB value ) const
        {
            return { R+value.R, G+value.G, B+value.B };
        }

        inline ColorBufferRGB& operator+=( const ColorBufferRGB value )
        {
            R += value.R;
            G += value.G;
            B += value.B;
            
            return *this;
        }
        
        inline ColorBufferRGB operator-( const int value ) const
        {
            return { R-value, G-value, B-value };
        }
        
        inline ColorBufferRGB& operator-=( const int value )
        {
            R -= value;
            G -= value;
            B -= value;
            
            return *this;
        }
        
        inline ColorBufferRGB operator-( const ColorBufferRGB value ) const
        {
            return { R-value.R, G-value.G, B-value.B };
        }
        
        inline ColorBufferRGB& operator-=( const ColorBufferRGB value )
        {
            R -= value.R;
            G -= value.G;
            B -= value.B;
            
            return *this;
        }
        
        inline ColorBufferRGB operator*( const int value ) const
        {
            return { R*value, G*value, B*value };
        }
        
        inline ColorBufferRGB operator*( const ColorBufferRGB value ) const
        {
            return { R*value.R, G*value.G, B*value.B };
        }

        inline ColorBufferRGB& operator*=( const int value )
        {
            R *= value;
            G *= value;
            B *= value;
            
            return *this;
        }
        
        inline ColorBufferRGB operator/( const int value ) const
        {
            return { R/value, G/value, B/value };
        }
        
        inline ColorBufferRGB& operator/=( const int value )
        {
            R /= value;
            G /= value;
            B /= value;
            
            return *this;
        }
        
        inline ColorBufferRGB& operator>>=( const int value )
        {
            R >>= value;
            G >>= value;
            B >>= value;
            
            return *this;
        }
        
        inline ColorBufferRGB operator>>( const int value ) const
        {
            return {R >> value, G >> value, B >> value};
        }
        
        inline operator RGB() const
        {
            return { (uint8_t)R, (uint8_t)G, (uint8_t)B };
        }

        inline RGB min( const int value ) const
        {
            return {
                (uint8_t)core::fast_min( R, value ),
                (uint8_t)core::fast_min( G, value  ),
                (uint8_t)core::fast_min( B, value  ) };
        }

        inline RGB max( const int value ) const
        {
            return {
                (uint8_t)core::fast_max( R, value ),
                (uint8_t)core::fast_max( G, value  ),
                (uint8_t)core::fast_max( B, value  ) };
        }

        inline RGB abs() const
        {
            return {
                (uint8_t)core::fast_max( R, -R ),
                (uint8_t)core::fast_max( G, -G ),
                (uint8_t)core::fast_max( B, -B ) };
        }
        
        inline RGB limit_min_max() const
        {
            return {
                (uint8_t)core::limit( R, 0, 255 ),
                (uint8_t)core::limit( G, 0, 255 ),
                (uint8_t)core::limit( B, 0, 255 ) };
        }

        inline RGB limit_min() const
        {
            return {
                (uint8_t)core::fast_max( R, 0 ),
                (uint8_t)core::fast_max( G, 0 ),
                (uint8_t)core::fast_max( B, 0 ) };
        }

        inline RGB limit_max() const
        {
            return {
                (uint8_t)core::fast_min( R, 255 ),
                (uint8_t)core::fast_min( G, 255 ),
                (uint8_t)core::fast_min( B, 255 ) };
        }
        
        inline static RGB compare_min( const RGB left, const RGB right )
        {
            return {
                (uint8_t)core::fast_min( left.R, right.R ),
                (uint8_t)core::fast_min( left.G, right.G ),
                (uint8_t)core::fast_min( left.B, right.B ) };
        }

        inline static RGB compare_max( const RGB left, const RGB right )
        {
            return {
                (uint8_t)core::fast_max( left.R, right.R ),
                (uint8_t)core::fast_max( left.G, right.G ),
                (uint8_t)core::fast_max( left.B, right.B ) };
        }
    };
    
    // Unimplemented
    struct ColorBufferRGBA
    {
        int R = 0;
        int G = 0;
        int B = 0;
        int A = 0;
    };
    
    // Unimplemented
    struct ColorBufferHMB
    {
        float H = 0.0f;
        float M = 0.0f;
        float B = 0.0f;
    };
#undef BaseColorBufferOperator3Channel
    
    inline static const uint8_t alpha_blend( const uint8_t back, const uint8_t fore, const int fixed_alpha )
    {
        const int a   = fixed_alpha;
        const int ia  = 1024 - a;
        
        return (uint8_t)((back * ia + fore * a) >> 10);
    }
    
    inline static const uint8_t alpha_blend( const uint8_t back, const uint8_t fore, const uint8_t alpha )
    {
        return alpha_blend( back, fore, core::fixed_point_table[ alpha ] );
    }
    
    inline static const uint8_t alpha_blend( const uint8_t back, const uint8_t fore, const float alpha )
    {
        return alpha_blend( back, fore, (int)(alpha*1024.0f) );
    }

    inline static const RGB alpha_blend( const RGB back, const RGB fore, const int alpha )
    {
        return
        {
            (uint8_t)alpha_blend( back.R, fore.R, alpha ),
            (uint8_t)alpha_blend( back.G, fore.G, alpha ),
            (uint8_t)alpha_blend( back.B, fore.B, alpha )
        };
    }
    
    enum class Interpolation
    {
        nearest,
        bilinear,
        bicubic,
        super,
    };
    
    using bicubic_table = std::array<int, 201>;
    
    static bicubic_table create_bicubic_table( const float a )
    {
        bicubic_table table;
        
        for( int d=0; d<=table.size(); ++d )
        {
            const float fd = d / 100.0f;
            
            if( d < 100 )
            {
                table[d] = (int)(((a+2.0f) * fd * fd * fd - (a+3.0f) * fd * fd + 1.0f) * 1000.0f);
            }
            else if( d < 200 )
            {
                table[d] = (int)((a * fd * fd * fd  - 5 * a * fd * fd + 8 * a * fd - 4 * a) * 1000.0f);
            }
            else
            {
                table[d] = 0.0f;
            }
        }
        
        return table;
    }
    
    namespace core
    {
        static const bicubic_table default_bicubic_table = create_bicubic_table(-1.0f);
        
#pragma pack(1)
        struct BitmapHeader
        {
            uint8_t  bfType1          = 'B';
            uint8_t  bfType2          = 'M';
            uint32_t bfSize           = 0;
            uint16_t bfReserved1      = 0;
            uint16_t bfReserved2      = 0;
            uint32_t bfOffBits        = 0;
            uint32_t biSize           = 40;
            int32_t  biWidth          = 0;
            int32_t  biHeight         = 0;
            uint16_t biPlanes         = 1;
            uint16_t biBitCount       = 0;
            uint32_t biCompression    = 0;
            uint32_t biSizeImage      = 0;
            int32_t  biXPelsPerMeter  = 0;
            int32_t  biYPelsPerMeter  = 0;
            uint32_t biClrUsed        = 0;
            uint32_t biClrImportant   = 0;
        };
#pragma pack()
        template <class T, class ColorBuffer> class Image
        {
        public:
            union
            {
                struct
                {
                    const int width = 0;
                    const int height = 0;
                };
                const SizeI size;
            };
            const unsigned int length = 0;
            
        private:
            std::vector< T > m_data;
           
        public:
            class Move
            {
                friend class Image;
            private:
                SizeI size = {};
                unsigned int length = 0;
                std::vector< T > m_data;
                
            public:
                Move( Image&& image )
                {
                    size   = image.size;
                    length = image.length;
                    m_data = std::move( image.m_data );
                    
                    (SizeI&)image.size = SizeI();
                    (unsigned int&)image.length = 0;
                }
            };
            
        public:
            Image() noexcept{}
            Image( const Image& value ) noexcept
            {
                (SizeI&)this->size = value.size;
                (unsigned int&)length = value.size.width*size.height;
                m_data = value.m_data;
            }
            Image( Image&& value ) noexcept{ *this = std::forward<Image>(value); }
            Image( Move&& value ) noexcept{ *this = std::forward<Move>(value); }
            Image( const SizeI& size ){ create( size ); }
            virtual ~Image(){}
            virtual void create( const SizeI& size )
            {
                (SizeI&)this->size = size;
                (unsigned int&)length = size.width*size.height;
                if( length == m_data.size() )
                {
                    fill({});
                }
                else
                {
                    m_data.resize( length );
                }
            }
            virtual void clear()
            {
                (SizeI&)size = SizeI();
                (unsigned int&)length = 0;
                m_data.clear();
            }
            inline T& operator[]( unsigned int index ){ return m_data[index]; };
            inline const T& operator[]( unsigned int index ) const{ return m_data[index]; };
            inline T& operator[]( const PointI& point ){ return m_data[point.y*width+point.x]; };
            inline const T& operator[]( const PointI& point ) const{ return m_data[point.y*width+point.x]; };
            inline typename std::vector<T>::iterator begin(){return m_data.begin();}
            inline typename std::vector<T>::iterator end(){return m_data.end();}
            inline typename std::vector<T>::const_iterator cbegin() const{return m_data.cbegin();}
            inline typename std::vector<T>::const_iterator cend() const{return m_data.cend();}

            Image<T,ColorBuffer>& operator=( const Image<T,ColorBuffer>& value )
            {
                clear();
                (SizeI&)size = value.size;
                (unsigned int&)length = value.length;
                m_data = value.m_data;
                return *this;
            }
            
            inline T get_nearest_pixel( const float x, const float y ) const
            {
                return (*this)[ { (int)(x+0.5f), (int)(y+0.5f) } ];
            }

            inline T get_nearest_pixel_safe_out_of_range( const float x, const float y ) const
            {
                return (*this)[ { limit( (int)(x+0.5f), 0, width-1), limit( (int)(y+0.5f), 0, height-1 ) } ];
            }
            
            inline T get_bilinear_pixel( const float x, const float y ) const
            {
                const int ix = (int)x;
                const int iy = (int)y;
                const int fx = (int)((1.0f - (x - (float)ix))*1024.0f);
                const int fy = (int)((1.0f - (y - (float)iy))*1024.0f);
                
                const T* base = &(*this)[ { ix+0, iy+0 } ];
                const ColorBuffer c1 = base[0];      // { ix+0, iy+0 }
                const ColorBuffer c2 = base[1];      // { ix+1, iy+0 }
                const ColorBuffer c3 = base[width];  // { ix+0, iy+1 }
                const ColorBuffer c4 = base[width+1];// { ix+1, iy+1 }

                return (((c1*fx + c2*(1024-fx))>>10) * fy + ((c3*fx + c4*(1024-fx))>>10) * (1024-fy)) >> 10;
            }

            inline T get_bilinear_pixel_safe_out_of_range( const float x, const float y ) const
            {
                const int ix = (int)x;
                const int iy = (int)y;
                const float fx = x >= 0.0f ? 1.0f - (x - (float)ix) : 1.0f + (x - (float)ix);
                const float fy = y >= 0.0f ? 1.0f - (y - (float)iy) : 1.0f + (y - (float)iy);
                const int aix = limit( ix, 0, width - 2 );
                const int aiy = limit( iy, 0, height- 2 );
                
                const T* base = &(*this)[ { aix+0, aiy+0 } ];
                const ColorBuffer c1 = base[0];      // { ix+0, iy+0 }
                const ColorBuffer c2 = base[1];      // { ix+1, iy+0 }
                const ColorBuffer c3 = base[width];  // { ix+0, iy+1 }
                const ColorBuffer c4 = base[width+1];// { ix+1, iy+1 }

                return (((c1*fx + c2*(1024-fx))>>10) * fy + ((c3*fx + c4*(1024-fx))>>10) * (1024-fy)) >> 10;
            }
            
            inline T get_bicubic_pixel( const float x, const float y, const bicubic_table& table=default_bicubic_table ) const
            {
                const int ix = (int)x;
                const int iy = (int)y;
                const float fx = x - (float)ix;
                const float fy = y - (float)iy;
                const int fx100 = (int)(fx * 100);
                const int fy100 = (int)(fy * 100);
                
                const int x0 = fast_max( ix-1, 0 );
                const int x1 = ix;
                const int x2 = fast_min( ix+1, width-1 );
                const int x3 = fast_min( ix+2, width-1 );

                const int y0 = fast_max( iy-1, 0 );
                const int y1 = iy;
                const int y2 = fast_min( iy+1, height-1 );
                const int y3 = fast_min( iy+2, height-1 );
                
                const ColorBuffer color[16] =
                {
                    (*this)[ {x0, y0} ], (*this)[ {x1, y0} ], (*this)[ {x2, y0} ], (*this)[ {x3, y0} ],
                    (*this)[ {x0, y1} ], (*this)[ {x1, y1} ], (*this)[ {x2, y1} ], (*this)[ {x3, y1} ],
                    (*this)[ {x0, y2} ], (*this)[ {x1, y2} ], (*this)[ {x2, y2} ], (*this)[ {x3, y2} ],
                    (*this)[ {x0, y3} ], (*this)[ {x1, y3} ], (*this)[ {x2, y3} ], (*this)[ {x3, y3} ],
                };
                
                const int ti_x0 = fx100 + 100;
                const int ti_x1 = fx100;
                const int ti_x2 = 100 - fx100;
                const int ti_x3 = 200 - fx100;

                const int ti_y0 = fy100 + 100;
                const int ti_y1 = fy100;
                const int ti_y2 = 100 - fy100;
                const int ti_y3 = 200 - fy100;

                const int weight_x = table[ ti_x0 ] + table[ ti_x1 ] + table[ ti_x2 ] + table[ ti_x3 ];
                const int weight_y = table[ ti_y0 ] + table[ ti_y1 ] + table[ ti_y2 ] + table[ ti_y3 ];
                
                const ColorBuffer result =
                    ((
                        color[ 0] * table[ ti_x0 ] + color[ 1] * table[ ti_x1 ] +
                        color[ 2] * table[ ti_x2 ] + color[ 3] * table[ ti_x3 ]
                    ) / weight_x ) * table[ ti_y0 ] +
                
                    ((
                        color[ 4] * table[ ti_x0 ] + color[ 5] * table[ ti_x1 ] +
                        color[ 6] * table[ ti_x2 ] + color[ 7] * table[ ti_x3 ]
                    ) / weight_x ) * table[ ti_y1 ] +
                
                    ((
                        color[ 8] * table[ ti_x0 ] + color[ 9] * table[ ti_x1 ] +
                        color[10] * table[ ti_x2 ] + color[11] * table[ ti_x3 ]
                    ) / weight_x ) * table[ ti_y2 ] +
                
                    ((
                        color[12] * table[ ti_x0 ] + color[13] * table[ ti_x1 ] +
                        color[14] * table[ ti_x2 ] + color[15] * table[ ti_x3 ]
                    ) / weight_x) * table[ ti_y3 ];
            
                return (result / weight_y).limit_min_max();
            }
            
            inline T get_bicubic_pixel_safe_out_of_range( const float x, const float y,
                                                         const bicubic_table& table=default_bicubic_table ) const
            {
                const int ix = (int)x;
                const int iy = (int)y;
                const float fx = x >= 0.0f ? 1.0f - (x - (float)ix) : 1.0f + (x - (float)ix);
                const float fy = y >= 0.0f ? 1.0f - (y - (float)iy) : 1.0f + (y - (float)iy);
                const int fx100 = (int)(fx * 100 + 100);
                const int fy100 = (int)(fy * 100 + 100);
                
                const int x0 = limit( ix-1, 0, width -  1);
                const int x1 = limit( ix  , 0, width -  1);
                const int x2 = limit( ix+1, 0, width -  1);
                const int x3 = limit( ix+2, 0, width -  1);
                
                const int y0 = limit( iy-1, 0, height - 1);
                const int y1 = limit( iy  , 0, height - 1);
                const int y2 = limit( iy+1, 0, height - 1);
                const int y3 = limit( iy+2, 0, height - 1);
                
                const ColorBuffer color[16] =
                {
                    (*this)[ {x0, y0} ], (*this)[ {x1, y0} ], (*this)[ {x2, y0} ], (*this)[ {x3, y0} ],
                    (*this)[ {x0, y1} ], (*this)[ {x1, y1} ], (*this)[ {x2, y1} ], (*this)[ {x3, y1} ],
                    (*this)[ {x0, y2} ], (*this)[ {x1, y2} ], (*this)[ {x2, y2} ], (*this)[ {x3, y2} ],
                    (*this)[ {x0, y3} ], (*this)[ {x1, y3} ], (*this)[ {x2, y3} ], (*this)[ {x3, y3} ],
                };
                
                const int ti_x0 = fx100 + 100;
                const int ti_x1 = fx100;
                const int ti_x2 = 100 - fx100;
                const int ti_x3 = 200 - fx100;
                
                const int ti_y0 = fy100 + 100;
                const int ti_y1 = fy100;
                const int ti_y2 = 100 - fy100;
                const int ti_y3 = 200 - fy100;
                
                const int weight_x = table[ ti_x0 ] + table[ ti_x1 ] + table[ ti_x2 ] + table[ ti_x3 ];
                const int weight_y = table[ ti_y0 ] + table[ ti_y1 ] + table[ ti_y2 ] + table[ ti_y3 ];
                
                const ColorBuffer result =
                    ((
                      color[ 0] * table[ ti_x0 ] + color[ 1] * table[ ti_x1 ] +
                      color[ 2] * table[ ti_x2 ] + color[ 3] * table[ ti_x3 ]
                      ) / weight_x ) * table[ ti_y0 ] +
                    
                    ((
                      color[ 4] * table[ ti_x0 ] + color[ 5] * table[ ti_x1 ] +
                      color[ 6] * table[ ti_x2 ] + color[ 7] * table[ ti_x3 ]
                      ) / weight_x ) * table[ ti_y1 ] +
                    
                    ((
                      color[ 8] * table[ ti_x0 ] + color[ 9] * table[ ti_x1 ] +
                      color[10] * table[ ti_x2 ] + color[11] * table[ ti_x3 ]
                      ) / weight_x ) * table[ ti_y2 ] +
                    
                    ((
                      color[12] * table[ ti_x0 ] + color[13] * table[ ti_x1 ] +
                      color[14] * table[ ti_x2 ] + color[15] * table[ ti_x3 ]
                      ) / weight_x) * table[ ti_y3 ];
                
                return (result / weight_y).limit_min_max();
            }
            
        private:
            void convert( const Image<GRAY,ColorBufferGRAY>& ){ throw std::bad_function_call(); }
            void convert( const Image<RGB,ColorBufferRGB>& ){ throw std::bad_function_call(); }
            void convert( const Image<HMB,ColorBufferHMB>& ){ throw std::bad_function_call(); }

        public:
            template <class C, class B>
            Image<T,ColorBuffer>& operator=( const Image<C,B>& image )
            {
                create( image.size );
                convert( image );
                return *this;
            }

            inline Image& operator=( Image&& value )
            {
                (SizeI&)size = value.size;
                (unsigned int&)length = value.length;
                (int&)value.width = 0;
                (int&)value.height = 0;
                (SizeI&)value.size = SizeI();
                (unsigned int&)value.length = 0;
                m_data = std::move( value.m_data );
                return *this;
            }
            
            inline Image& operator=( Move&& value )
            {
                (SizeI&)size = value.size;
                (unsigned int&)length = value.length;
                (SizeI&)value.size = SizeI();
                (unsigned int&)value.length = 0;
                m_data = std::move( value.m_data );
                return *this;
            }
            
            inline void fill(const T color)
            {
                std::fill( m_data.begin(), m_data.end(), color );
            }
           
            void fill(const T color, const RectangleI& rectangle)
            {
                const int begin_x = fast_max( rectangle.point.x, 0 );
                const int end_x   = fast_min( rectangle.point.x + rectangle.size.width, width );
                const int begin_y = fast_max( rectangle.point.y, 0 );
                const int end_y   = fast_min( rectangle.point.y + rectangle.size.height, height );
                
                for( int y = begin_y; y<end_y; ++y )
                {
                    for( int x= begin_x; x<end_x; ++x )
                    {
                        (*this)[ {x, y} ] = color;
                    }
                }
            }

            void fill(const T color, const CircleF& circle)
            {
                const int begin_x = fast_max( (int)(circle.point.x - circle.radius-1),      0 );
                const int end_x   = fast_min( (int)(circle.point.x + circle.radius+1),  width );
                const int begin_y = fast_max( (int)(circle.point.y - circle.radius-1),      0 );
                const int end_y   = fast_min( (int)(circle.point.y + circle.radius+1), height );
                const float radius_square = circle.radius * circle.radius;
                
                for( int y = begin_y; y<end_y; ++y )
                {
                    const float sub_y = y - circle.point.y;
                    const float square_y = sub_y * sub_y;

                    for( int x= begin_x; x<end_x; ++x )
                    {
                        const float sub_x = x - circle.point.x;
                        const float square_x = sub_x * sub_x;
                        
                        if( square_x + square_y <= radius_square )
                        {
                            (*this)[ {x, y} ] = color;
                        }
                    }
                }
            }
            
            Move trimming( const RectangleI& rectangle ) const
            {
                Image output( rectangle.size );

                for( int y=0; y<output.height; ++y )
                {
                    memcpy(
                           &output[{0,y}],
                           &(*this)[{rectangle.point.x,rectangle.point.y+y}],
                           sizeof(T)*rectangle.size.width );
                }
                return std::move(output);
            }
    
            Move resize( const SizeI& size, const Interpolation interpo = Interpolation::bicubic ) const
            {
                if( size.width <= 0 )
                {
                    throw std::range_error( "resize : size.width <= 0" );
                }
                if( size.height <= 0)
                {
                    throw std::range_error( "resize : size.height <= 0" );
                }
                
                Image output( size );
                
                if( this->size == size )
                {
                    output = *this;
                }
                else
                {
                    const float x_step = (float)(width  - 1) / (size.width  - 1);
                    const float y_step = (float)(height - 1) / (size.height - 1);
                    
                    float y_pos = 0;
                    T* p_output = &output[0];
                    
                    if( interpo == Interpolation::nearest )
                    {
                        for( int y=0; y<size.height; ++y, y_pos += y_step )
                        {
                            float x_pos = 0;
                            for( int x=0; x<size.width; ++x, x_pos += x_step, ++p_output )
                            {
                                *p_output = get_nearest_pixel( x_pos, y_pos );
                            }
                        }
                    }
                    if( interpo == Interpolation::bilinear )
                    {
                        for( int y=0; y<size.height; ++y, y_pos += y_step )
                        {
                            float x_pos = 0;
                            for( int x=0; x<size.width; ++x, x_pos += x_step, ++p_output )
                            {
                                *p_output = get_bilinear_pixel( x_pos, y_pos );
                            }
                        }
                    }
                    if( interpo == Interpolation::bicubic )
                    {
                        for( int y=0; y<size.height; ++y, y_pos += y_step )
                        {
                            float x_pos = 0;
                            for( int x=0; x<size.width; ++x, x_pos += x_step, ++p_output )
                            {
                                *p_output = get_bicubic_pixel( x_pos, y_pos );
                            }
                        }
                    }
                    if( interpo == Interpolation::super )
                    {
                        int y_pos_super =  0;

                        const int x_step_super = (int)((float)width  / (size.width)  * 1024);
                        const int y_step_super = (int)((float)height / (size.height) * 1024);

                        for( int y=0; y<size.height; ++y, y_pos_super += y_step_super )
                        {
                            int x_pos_super = 0;

                            for( int x=0; x<size.width; ++x, x_pos_super += x_step_super, ++p_output )
                            {
                                ColorBuffer color;
                                int weight = 0;
                                int y_rem = 1024 - (y_pos_super - (y_pos_super&0xFFFFFC00));
                                int y_area = y_step_super;
                                
                                for( int yy=(y_pos_super>>10); y_area > 0; ++yy )
                                {
                                    int x_rem = 1024 - (x_pos_super - (x_pos_super&0xFFFFFC00));

                                    if( y_area <= 1024 )
                                    {
                                        y_rem = y_area;
                                    }
                                    y_area -= y_rem;
                                    
                                    int x_area = x_step_super;
                                    
                                    for( int xx=(x_pos_super>>10); x_area > 0; ++xx )
                                    {
                                        if( x_area <= 1024 )
                                        {
                                            x_rem = x_area;
                                        }
                                        color += ColorBuffer( (*this)[{xx,yy}] ) * ((x_rem*y_rem)>>10);
                                        weight += ((x_rem*y_rem) >> 10);
                                        x_area -= x_rem;
                                        x_rem = 1024;
                                    }
                                    y_rem = 1024;
                                }
                                *p_output = color / weight;
                            }
                        }
                    }
                }
                return std::move(output);
            }

            inline Move resize( const float scaling, const Interpolation interpo = Interpolation::bicubic ) const
            {
                return resize( {width*scaling+0.5f, height*scaling+0.5f}, interpo );
            }
            
            Move mirror_border( const int width_radius, const int height_radius ) const
            {
                if( width_radius > width )
                {
                    throw std::range_error( "mirror_border : width_radius > width" );
                }
                if( height_radius > height )
                {
                    throw std::range_error( "mirror_border : height_radius > height" );
                }
                Image output( {width+width_radius*2, height+height_radius*2} );
                
                int x, y;
                for( y=-height_radius; y<height; ++y )
                {
                    for( x=-width_radius; x<width; ++x )
                    {
                        output[ {x+width_radius,y+height_radius} ] = (*this)[{fast_abs(x), fast_abs(y)}];
                    }

                    for( int sx = x-1; x<output.width; ++x, --sx )
                    {
                        output[ {x+width_radius,y+height_radius} ] = (*this)[{sx, fast_abs(y)}];
                    }
                }

                int sy = y - 1;
                y += height_radius;
                
                for( ; y<output.height; ++y, --sy )
                {
                    for( x=-width_radius; x<width; ++x )
                    {
                        output[ {x+width_radius,y} ] = (*this)[{fast_abs(x), sy}];
                    }
                    for( int sx = x-1; x<output.width; ++x, --sx )
                    {
                        output[ {x,y} ] = (*this)[{sx, sy}];
                    }
                }
                return std::move(output);
            }
            
            Move filter( const std::vector< float >& kernel ) const
            {
                const int side = (int)sqrt( kernel.size() );
                const int radius = side / 2;
                
                Image output( size );
                Image temp = mirror_border( radius, radius );
                std::vector< int > ikernel( kernel.size() );
                
                for( size_t i=0; i<kernel.size(); ++i )
                {
                    ikernel[i] = (int)(kernel[i] * 1024.0f);
                }
                
                for( int y=0; y<height; ++y )
                {
                    for( int x=0; x<width; ++x )
                    {
                        ColorBuffer color;
                        for( int i=0; i<side; ++i )
                        {
                            const int tx = x+i;
                            
                            for( int j=0; j<side; ++j )
                            {
                                const int ty = y+j;
                                
                                color += temp[ {tx, ty} ] * ikernel[ i*side + j ];
                            }
                        }
                        output[ {x,y} ] = (color >> 10).limit();
                    }
                }
                return std::move( output );
            }
            
            Move gaussian( const float sigma ) const
            {
                if( sigma < 0.0f )
                {
                    throw std::range_error( "gaussian : sigma < 0.0f" );
                }
                
                Image output( size );
                
                const float pixel_distance_scale = 2.0f;
                const int radius = (int)( sigma / pixel_distance_scale * 2.0f );
                
                if( radius == 0 )
                {
                    output = *this;
                }
                else
                {
                    std::vector< int > kernel( radius * 2 + 1 );
                    
                    const double sigma_2_square = 2 * sigma * sigma;
                    double root_sigma_square_pi = sqrt(2.0 * M_PI * sigma * sigma);
                    
                    double sum = 0.0;
                    for( int i=0; i<(int)kernel.size(); ++i )
                    {
                        double distance = (i-radius) * pixel_distance_scale;

                        sum += exp( -(distance*distance / sigma_2_square) ) / root_sigma_square_pi;
                    }
                    
                    const double weight = 4096.0 / sum;
                    for( int i=0; i<(int)kernel.size(); ++i )
                    {
                        double distance = (i-radius) * pixel_distance_scale;

                        kernel[i] = (int)(
                            (exp( -(distance*distance / sigma_2_square) ) / root_sigma_square_pi ) * weight);
                    }
                    
                    const Image input = mirror_border(radius, radius);
                    std::vector<ColorBuffer> horizontal( input.width );
                    
                    const T* p_input = &input[0];
                    T* p_output = &output[0];

                    for( int y=0; y<output.height; ++y )
                    {
                        // vertical filter.
                        for( int x=0; x<input.width; ++x, ++p_input )
                        {
                            const T* p_input_xy = p_input;

                            ColorBuffer color;
                            for( int i=0; i<kernel.size(); ++i, p_input_xy += input.width )
                            {
                                color += ColorBuffer(*p_input_xy) * kernel[ i ];
                            }
                            horizontal[x] = color >> 6;
                        }
                        
                        // horizontal filter.
                        for( int x=0; x<output.width; ++x, ++p_output )
                        {
                            ColorBuffer color;
                            for( int i=0; i<kernel.size(); ++i )
                            {
                                color += ColorBuffer( horizontal[i+x] ) * kernel[ i ];
                            }
                            *p_output = color >> 18;
                        }
                    }
                }
            
                return std::move( output );
            }

#define BlendConcept( FORE ) \
            Image output( size ); \
            \
            const int ialpha = (int)(alpha * 1024.0f); \
            const Image& back = *this; \
            \
            for( unsigned int i=0; i<length; ++i ) \
            { \
                output[i] = gs::alpha_blend( back[i], FORE, ialpha ); \
            } \
            \
            return std::move(output)
            
#define fore_color ColorBuffer(fore[i])
#define back_color ColorBuffer(back[i])
#define args_color ColorBuffer(color)

            Move alpha_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( fore_color );
            }

            Move alpha_blend( const T color, float alpha ) const
            {
                BlendConcept( args_color );
            }
            
            Move addition_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( ( back_color + fore_color ).limit_max() );
            }

            Move addition_blend( const T color, float alpha ) const
            {
                BlendConcept( ( back_color + args_color ).limit_max() );
            }
            
            Move subtract_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( ( back_color - fore_color ).limit_min() );
            }
            
            Move multiply_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( back_color * fore_color / 255 );
            }
            
            Move difference_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( ( fore_color - back_color ).abs() );
            }
            
            Move color_burn_blend( const Image& fore, float alpha ) const
            {
                BlendConcept(
                    (ColorBuffer(255) -
                     (( ColorBuffer(255) - back_color ) * 255 / fore_color.max(1) )).limit_min()
                );
            }
            
            Move darken_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( ColorBuffer::compare_min( back_color, fore_color ) );
            }
            
            Move lighten_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( ColorBuffer::compare_max( back_color, fore_color ) );
            }

            Move linear_burn_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( (back_color + fore_color - 255).limit_min() );
            }

            Move screen_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( back_color + fore_color - back_color * fore_color / 255 );
            }

            Move color_dodge_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( (back_color * 255 / (ColorBuffer(255) - fore_color).max(1)).limit_max() );
            }

            Move exclusion_blend( const Image& fore, float alpha ) const
            {
                BlendConcept( ( back_color + fore_color - 2 * back_color * fore_color / 255).limit_min() );
            }
            
#undef BlendConcept
#undef fore_color
#undef back_color
#undef args_color

#define BlendConceptChannel( FORE ) \
    Image output( size ); \
    \
    const int ialpha = (int)(alpha * 1024.0f); \
    const Image& back = *this; \
    \
    for( unsigned int i=0; i<length; ++i ) \
    { \
        for( int c=0; c<T::CHANNEL; ++c ) \
        { \
            output[i][c] = gs::alpha_blend( back[i][c], FORE, ialpha ); \
        } \
    } \
    \
    return std::move(output)
            
#define fore_color (fore[i][c])
#define back_color (back[i][c])
#define args_color (color[c])

            Move overlay_blend( const Image& fore, float alpha ) const
            {
                BlendConceptChannel( back_color < 128 ?
                             back_color*fore_color*2/255 :
                             2*(back_color+fore_color-back_color*fore_color/255)-255 );
            }

            Move overlay_blend( const T color, float alpha ) const
            {
                BlendConceptChannel( back_color < 128 ?
                                    back_color*args_color*2/255 :
                                    2*(back_color+args_color-back_color*args_color/255)-255 );
            }
            
            Move soft_light_blend( const Image& fore, float alpha ) const
            {
                BlendConceptChannel( fore_color < 128 ?
                             std::pow( back_color / 255.0f, 2.0f * (1.0f - fore_color / 255.0f) ) * 255.0f :
                             std::pow( back_color / 255.0f, 2.0f * (1.0f / (2.0f * fore_color / 255.0f) ) ) * 255.0f );
            }

            Move soft_light_blend( const T color, float alpha ) const
            {
                BlendConceptChannel( args_color < 128 ?
                                    std::pow( back_color / 255.0f, 2.0f * (1.0f - args_color / 255.0f) ) * 255.0f :
                                    std::pow( back_color / 255.0f, 2.0f * (1.0f / (2.0f * args_color / 255.0f) ) ) * 255.0f );
            }
            
            Move hard_light_blend( const Image& fore, float alpha ) const
            {
                BlendConceptChannel( fore_color < 128 ?
                             back_color*fore_color*2/255 :
                             2*(back_color+fore_color-back_color*fore_color/255) - 255 );
            }

            Move hard_light_blend( const T color, float alpha ) const
            {
                BlendConceptChannel( args_color < 128 ?
                                    back_color*args_color*2/255 :
                                    2*(back_color+args_color-back_color*args_color/255) - 255 );
            }
            
            Move vivid_light_blend( const Image& fore, float alpha ) const
            {
                BlendConceptChannel( fore_color < 128 ?
                                 back_color < 255-2*fore_color ?
                                    0 :
                                    (back_color-(255-2*fore_color)) / (2 * fast_max(fore_color,1)) :
                                 back_color < 2*(255-fore_color) ?
                                    back_color / 2*(255-fore_color) :
                                    255
                             );
            }

            Move vivid_light_blend( const T color, float alpha ) const
            {
                BlendConceptChannel( args_color < 128 ?
                                back_color < 255-2*args_color ?
                                    0 :
                                    (back_color-(255-2*args_color)) / (2 * fast_max(args_color,1)) :
                                back_color < 2*(255-args_color) ?
                                    back_color / 2*(255-args_color) :
                                    255
                                    );
            }
            
            Move linear_light_blend( const Image& fore, float alpha ) const
            {
                BlendConceptChannel( fore_color < 128 ?
                                 back_color < 255 - 2 * fore_color ?
                                    0 :
                                    fast_min( 2 * fore_color + back_color + 255, 255 ) :
                             
                                 back_color < 2 * (255-fore_color) ?
                                    fast_min( 2 * fore_color + back_color + 255, 255 ) :
                                    255
                             );
            }

            Move linear_light_blend( const T color, float alpha ) const
            {
                BlendConceptChannel( args_color < 128 ?
                                    back_color < 255 - 2 * args_color ?
                                        0 :
                                        fast_min( 2 * args_color + back_color + 255, 255 ) :
                                    
                                    back_color < 2 * (255-args_color) ?
                                        fast_min( 2 * args_color + back_color + 255, 255 ) :
                                        255
                                    );
            }
            
            Move pin_light_blend( const Image& fore, float alpha ) const
            {
                BlendConceptChannel( fore_color < 128 ?
                                 back_color < 255 - 2 * fore_color ?
                                    back_color :
                                    2 * fore_color :
                                 
                                 back_color < 2 * fore_color - 255 ?
                                    2 * fore_color - 255 :
                                    back_color
                             );
            }

            Move pin_light_blend( const T color, float alpha ) const
            {
                BlendConceptChannel( args_color < 128 ?
                                    back_color < 255 - 2 * args_color ?
                                    back_color :
                                    2 * args_color :
                                    
                                    back_color < 2 * args_color - 255 ?
                                    2 * args_color - 255 :
                                    back_color
                                    );
            }
            
#undef BlendConceptChannel
#undef fore_color
#undef back_color
#undef args_color

            void write( std::ostream& output ) const
            {
                output.clear();
                
                core::BitmapHeader header;
                const char paddingData[3] = {};
                const int padding = (4-(width*sizeof(T)%4))%4;
                
                header.biWidth     = width;
                header.biHeight    = height;
                header.biBitCount  = sizeof(T)*8;
                header.biSizeImage = length*sizeof(T);
                header.bfOffBits   = sizeof(core::BitmapHeader);
                if( header.biBitCount == 8 )
                {
                    header.bfOffBits += 4 * 256; //ColorTable
                }
                header.bfSize = header.bfOffBits + header.biSizeImage;
                
                output.write( (char*)&header, sizeof(core::BitmapHeader) );
                
                if( header.biBitCount == 8 ) // ColorTable
                {
                    uint8_t table[4] = { 0, 0, 0, 255 };
                    for( int i=0; i<256; ++i )
                    {
                        table[0] = table[1] = table[2] = i;
                        output.write( (char*)table, 4 );
                    }
                }
                
                for( int y=height-1; y>=0; --y )
                {
                    const char* data = (char*)&(*this)[{0,y}];
                    for( int x=0; x<width; ++x )
                    {
                        for( int i=T::CHANNEL-1; i>=0; --i )
                        {
                            output.write( data+i, 1 );
                        }
                        data += T::CHANNEL;
                    }
                    if( padding != 0 )
                    {
                        output.write( paddingData, padding );
                    }
                }
            }
            
            void write( const std::string& filepath ) const
            {
                std::fstream file;
                file.open( filepath, std::ios::out | std::ios::binary );
                write( file );
                file.close();
            }
            
            inline Image& operator>>( const std::string& filepath )
            {
                write( filepath );
                return *this;
            }

            void read( const std::vector< uint8_t >& image_data )
            {
                const core::BitmapHeader* header = (const core::BitmapHeader*)&image_data[0];
                int index = sizeof(core::BitmapHeader);
                
                if( header->bfOffBits != 0 )
                {
                    index = header->bfOffBits;
                }
                if( header->biBitCount == 8 ) // ColorTable
                {
                    index += 4*256;
                }
                
                create( {header->biWidth, header->biHeight} );
                
                const int padding = (4-(width*sizeof(T)%4))%4;
                const int byteCount = header->biBitCount/8;
                
                for( int y=height-1; y>=0; --y )
                {
                    char* data = (char*)&(*this)[{0,y}];
                    for( int x=0; x<width; ++x )
                    {
                        for( int i=T::CHANNEL-1; i>=0; --i )
                        {
                            data[i] = image_data[index++];
                        }
                        data += byteCount;
                    }
                    index += padding;
                }
            }

            void read( std::istream& stream )
            {
                core::BitmapHeader header;
                
                stream.read( (char*)&header, sizeof(core::BitmapHeader) );
                
                if( header.bfOffBits != 0 )
                {
                    stream.seekg( header.bfOffBits, std::ios_base::beg );
                }
                if( header.biBitCount == 8 ) // ColorTable
                {
                    stream.seekg( 4*256, std::ios_base::cur );
                }
                
                create( {header.biWidth, header.biHeight} );
                
                const int padding = (4-(width*sizeof(T)%4))%4;
                const int byteCount = header.biBitCount/8;
                
                for( int y=height-1; y>=0; --y )
                {
                    char* data = (char*)&(*this)[{0,y}];
                    for( int x=0; x<width; ++x )
                    {
                        for( int i=T::CHANNEL-1; i>=0; --i )
                        {
                            stream.read( data+i, 1 );
                        }
                        data += byteCount;
                    }
                    if( padding != 0 )
                    {
                        stream.seekg( padding, std::ios_base::cur );
                    }
                }
            }
            
            void read( const std::string& filepath )
            {
                std::ifstream file;
                
                file.open( filepath, std::ios::in | std::ios::binary );
                read( file );
                file.close();
            }
            
            inline Image& operator<<(const std::string& filepath)
            {
                read( filepath );
                return *this;
            }
        };

        template<> void Image<GRAY, ColorBufferGRAY>::convert( const Image<RGB, ColorBufferRGB>& image )
        {
            create( image.size );
            for( unsigned int i=0; i<length; ++i )
            {
                const RGB rgb = image[i];
                (*this)[i].L = (uint8_t)((rgb.R*306 + rgb.G*601 + rgb.B * 117) >> 10);
            }
        }
        
        template<> void Image<RGB, ColorBufferRGB>::convert( const Image<GRAY, ColorBufferGRAY>& image )
        {
            create( image.size );
            for( unsigned int i=0; i<length; ++i )
            {
                const auto gray = image[i];
                (*this)[i] = { gray.L, gray.L, gray.L };
            }
        }

        template<> void Image<RGB, ColorBufferRGB>::convert( const Image<HMB, ColorBufferHMB>& image )
        {
            const RGB table[7] = {
                {255,  0,  0}, {255,255,  0}, {  0,255,  0}, {  0,255,255},
                {  0,  0,255}, {255,  0,255}, {255,  0,  0},
            };
            
            RGB color;
            create( image.size );
            for( unsigned int i=0; i<length; ++i )
            {
                const HMB hmb = image[i];
                
                const float angle         = hmb.H + (360.0f*2.0f); // Make it a positive number.
                const int   angle_integer = (int)(angle / 60.0f);
                const float alpha         = (angle - angle_integer * 60.0f) / 60.0f;
                const int   index         = angle_integer % 6;
                const float f_magnitude   = hmb.M / 255.0f;
                const float R = (table[index].R * (1.0f - alpha) + table[index+1].R * alpha) * f_magnitude;
                const float G = (table[index].G * (1.0f - alpha) + table[index+1].G * alpha) * f_magnitude;
                const float B = (table[index].B * (1.0f - alpha) + table[index+1].B * alpha) * f_magnitude;
                
                color.R = fast_min( (int)(R + hmb.B), 255 );
                color.G = fast_min( (int)(G + hmb.B), 255 );
                color.B = fast_min( (int)(B + hmb.B), 255 );
                
                (*this)[i] = color;
            }
        }

        template<> void Image<HMB, ColorBufferHMB>::convert( const Image<RGB, ColorBufferRGB>& image )
        {
            const Vector2   degree0_R(  1.0f,  0.0f      );
            const Vector2 degree120_G( -0.5f,  0.866025f );
            const Vector2 degree240_B( -0.5f, -0.866025f );
            
            HMB color;
            create( image.size );
            for( unsigned int i=0; i<length; ++i )
            {
                const RGB rgb = image[i];
                const float base = (float)fast_min( rgb.R, fast_min( rgb.G, rgb.B) );
                const float R = rgb.R - base;
                const float G = rgb.G - base;
                const float B = rgb.B - base;
                const Vector2 vec = (degree0_R * R) + (degree120_G * G) + (degree240_B * B);
                
                color.H = core::radian_to_degree( atan2( vec.y, vec.x ) );
                color.M = fast_max( rgb.R, fast_max( rgb.G, rgb.B) ) - base;
                color.B = base;
                
                (*this)[i] = color;
            }
        }
    }
    using ImageGRAY   = core::Image< GRAY  , ColorBufferGRAY   >;
    using ImageGRAY_F = core::Image< GRAY_F, ColorBufferGRAY_F >;
    using ImageRGB    = core::Image<  RGB  , ColorBufferRGB    >;
    using ImageARGB   = core::Image< RGBA  , ColorBufferRGBA   >;
    using ImageHMB    = core::Image<  HMB  , ColorBufferHMB    >;
    
    static inline ImageRGB::Move gaussian_keep_edge_hmb( const ImageRGB& image,
            const float sigma, const float hue, const float magnitude, const float base_luminance )
    {
        if( sigma < 0.0f )
        {
            throw std::range_error( "gaussian : sigma < 0.0f" );
        }
        
        const float pixel_distance_scale = 2.0f;
        const int radius = (int)( sigma / pixel_distance_scale * 2.0f );
        
        ImageRGB output( image.size );

        if( radius != 0 )
        {
            std::vector< int > kernel( radius * 2 + 1 );
            
            const double sigma_2_square = 2 * sigma * sigma;
            double root_sigma_square_pi = sqrt(2.0 * M_PI * sigma * sigma);
            
            double sum = 0.0;
            for( int i=0; i<(int)kernel.size(); ++i )
            {
                double distance = (i-radius) * pixel_distance_scale;
                
                sum += exp( -(distance*distance / sigma_2_square) ) / root_sigma_square_pi;
            }
            
            const double weight = 4096.0 / sum;
            for( int i=0; i<(int)kernel.size(); ++i )
            {
                double distance = (i-radius) * pixel_distance_scale;
                
                kernel[i] = (int)(
                                  (exp( -(distance*distance / sigma_2_square) ) / root_sigma_square_pi ) * weight);
            }
            
            ImageRGB input;
            ImageHMB hmb;
            
            input = image.mirror_border(radius, radius);
            hmb = input;
            
            const RGB* p_input = &input[0];
            const HMB* p_hmb = &hmb[0];
            RGB* p_output = &output[0];
            
            ImageRGB horizontal_rgb( {input.width, 1 } );
            ImageHMB horizontal_hmb( {input.width, 1 } );
            
            for( int y=0; y<output.height; ++y )
            {
                // vertical filter.
                for( int x=0; x<input.width; ++x, ++p_input, ++p_hmb )
                {
                    gs::HMB center_color_hmb = hmb[ {x,y+radius} ];
                    
                    const gs::RGB* p_input_xy = p_input;
                    const gs::HMB* p_hmb_xy = p_hmb;
                    
                    gs::ColorBufferRGB color;
                    int much_weight = 0;
                    for( int i=0; i<kernel.size(); ++i, p_input_xy += input.width, p_hmb_xy += hmb.width )
                    {
                        if( std::abs(center_color_hmb.H - (*p_hmb_xy).H) <= hue &&
                            std::abs(center_color_hmb.M - (*p_hmb_xy).M) <= magnitude &&
                            std::abs(center_color_hmb.B - (*p_hmb_xy).B) <= base_luminance )
                        {
                            color += ColorBufferRGB(*p_input_xy) * kernel[ i ];
                            much_weight += kernel[i];
                        }
                    }
                    horizontal_rgb[x] = color / much_weight;
                }
                
                horizontal_hmb = horizontal_rgb;
                
                // horizontal filter.
                for( int x=0; x<output.width; ++x, ++p_output )
                {
                    HMB center_color_hmb = horizontal_hmb[ x+radius ];
                    
                    ColorBufferRGB color;
                    int much_weight = 0;
                    for( int i=0; i<kernel.size(); ++i )
                    {
                        if( std::abs(center_color_hmb.H - horizontal_hmb[x+i].H) <= hue &&                            std::abs(center_color_hmb.M - horizontal_hmb[x+i].M) <= magnitude &&
                            std::abs(center_color_hmb.B - horizontal_hmb[x+i].B) <= base_luminance )
                        {
                            color += gs::ColorBufferRGB(horizontal_rgb[i+x]) * kernel[ i ];
                            much_weight += kernel[i];
                        }
                    }
                    *p_output = color / much_weight;
                }
            }
        }
        else
        {
            output = image;
        }
        
        return std::move( output );
    }
    
    static inline ImageRGB::Move gaussian_keep_edge_rgb( const ImageRGB& image,
                                                    const float sigma, const uint8_t r, const uint8_t g, const uint8_t b )
    {
        if( sigma < 0.0f )
        {
            throw std::range_error( "gaussian : sigma < 0.0f" );
        }
        
        const float pixel_distance_scale = 2.0f;
        const int radius = (int)( sigma / pixel_distance_scale * 2.0f );
        
        ImageRGB output( image.size );
        
        if( radius != 0 )
        {
            std::vector< int > kernel( radius * 2 + 1 );
            
            const double sigma_2_square = 2 * sigma * sigma;
            double root_sigma_square_pi = sqrt(2.0 * M_PI * sigma * sigma);
            
            double sum = 0.0;
            for( int i=0; i<(int)kernel.size(); ++i )
            {
                double distance = (i-radius) * pixel_distance_scale;
                
                sum += exp( -(distance*distance / sigma_2_square) ) / root_sigma_square_pi;
            }
            
            const double weight = 4096.0 / sum;
            for( int i=0; i<(int)kernel.size(); ++i )
            {
                double distance = (i-radius) * pixel_distance_scale;
                
                kernel[i] = (int)(
                                  (exp( -(distance*distance / sigma_2_square) ) / root_sigma_square_pi ) * weight);
            }
            
            ImageRGB input;
            
            input = image.mirror_border(radius, radius);
            
            const RGB* p_input = &input[0];
            RGB* p_output = &output[0];
            
            ImageRGB horizontal_rgb( {input.width, 1 } );
            
            for( int y=0; y<output.height; ++y )
            {
                // vertical filter.
                for( int x=0; x<input.width; ++x, ++p_input )
                {
                    RGB center_color_rgb = input[ {x,y+radius} ];
                    
                    const RGB* p_input_xy = p_input;
                    
                    ColorBufferRGB color;
                    int much_weight = 0;
                    for( int i=0; i<kernel.size(); ++i, p_input_xy += input.width )
                    {
                        if( core::fast_abs(center_color_rgb.R - (*p_input_xy).R) <= r &&
                            core::fast_abs(center_color_rgb.G - (*p_input_xy).G) <= g &&
                            core::fast_abs(center_color_rgb.B - (*p_input_xy).B) <= b )
                        {
                            color += ColorBufferRGB(*p_input_xy) * kernel[ i ];
                            much_weight += kernel[i];
                        }
                    }
                    horizontal_rgb[x] = color / much_weight;
                }
                
                // horizontal filter.
                for( int x=0; x<output.width; ++x, ++p_output )
                {
                    RGB center_color_rgb = horizontal_rgb[ x+radius ];
                    
                    ColorBufferRGB color;
                    int much_weight = 0;
                    for( int i=0; i<kernel.size(); ++i )
                    {
                        if( core::fast_abs(center_color_rgb.R - horizontal_rgb[x+i].R) <= r &&
                            core::fast_abs(center_color_rgb.G - horizontal_rgb[x+i].G) <= g &&
                            core::fast_abs(center_color_rgb.B - horizontal_rgb[x+i].B) <= b )
                        {
                            color += gs::ColorBufferRGB(horizontal_rgb[i+x]) * kernel[ i ];
                            much_weight += kernel[i];
                        }
                    }
                    *p_output = color / much_weight;
                }
            }
        }
        else
        {
            output = image;
        }
        
        return std::move( output );
    }
    
    static inline ImageRGB::Move restore_material(
            const ImageRGB& blur_image, const ImageRGB& original_image, const float strength )
    {
        if( strength < 0.0f )
        {
            throw std::range_error( "strength < 0.0f" );
        }
        if( strength > 1.0f )
        {
            throw std::range_error( "strength > 1.0f" );
        }
        if( blur_image.size != original_image.size )
        {
            throw std::range_error( "blur_image.size != original_image.size" );
        }
        
        ImageRGB output( blur_image.size );

        int istrength = (int)(strength * 1024.0f);
        
        for( int i=0; i<output.length; ++i )
        {
            const int min1 =
                core::fast_min( blur_image[i].R, core::fast_min(blur_image[i].G, blur_image[i].B) );
            const int min2 =
                core::fast_min( original_image[i].R, core::fast_min(original_image[i].G, original_image[i].B) );
            
            const int add = ((min2 - min1) * istrength) >> 10;
            
            output[i] = (gs::ColorBufferRGB(blur_image[i]) + add).limit_min_max();
        }
        
        return std::move( output );
    }
    /*
     *  [return] correct image.
     *
     *  [in]image
     *　　　　process image.
     *
     *  [in]temperature
     *　　　　kelvin correct. [-1.0] - [1.0]
     *       -1.0 Red. 0.0 White. 1.0 Blue
     *
     *  [in]strength [0.0] - [1.0]
     */
    static inline ImageRGB::Move correct_color_temperature( const ImageRGB& image, const float temperature, const float strength )
    {
        if( temperature < -1.0f )
        {
            throw std::range_error( "correct_color_temperature : correct_color_temperature < -1.0f" );
        }
        if( temperature > 1.0f )
        {
            throw std::range_error( "correct_color_temperature : correct_color_temperature > 1.0f" );
        }
        if( strength < 0.0f )
        {
            throw std::range_error( "correct_color_temperature : strength < 0.0f" );
        }
        if( strength > 1.0f )
        {
            throw std::range_error( "correct_color_temperature : strength > 1.0f" );
        }
        
        const RGB table[6] = {
            {255,  0,  0}, {255,255,  0}, {255,255,255},
            {  0,255,255}, {  0,  0,255}, {  0,  0,255}, /* last data is sentinel. */
        };
        
        ImageRGB output;
        
        if( temperature == 0.0f )
        {
            output = image;
        }
        else
        {
            int index = 0;
            int alpha = 0;
            if( temperature < 0.0f )
            {
                index = (1.0f+temperature) / 0.5f;
                alpha = (int)(((1.0f+temperature) - index*0.5f) / 0.5f * 1024.0f);
            }
            else
            {
                index = temperature / 0.5f + 2;
                alpha = (int)((temperature - (index-2)*0.5f) / 0.5f * 1024.0f);
            }
            
            output = image.alpha_blend( gs::alpha_blend( table[index], table[index+1], alpha ), strength );
        }
        return std::move( output );
    }
    
    static inline ImageHMB::Move edge_detection( const ImageRGB& image, const int radius )
    {
        const int side = radius * 2 + 1;
        std::vector< Vector2 > direction( side*side );
        
        int index = 0;
        for( int y=0; y<side; ++y )
        {
            for( int x=0; x<side; ++x, ++index )
            {
                Vector2 v( x-radius, y-radius );
                
                if( v.x == 0.0f && v.y == 0.0f )
                {
                    direction[index] = Vector2( 0,0 );
                }
                else
                {
                    direction[index] = v.normalize();
                }
            }
        }

        ImageGRAY input;
        ImageHMB output( image.size );
        
        input = image;
        input = input.mirror_border(radius, radius);
        
        const float max_distance_inverse = 1.0 / ( sqrt( radius*radius + radius*radius ) * radius );

        HMB* p_output = &output[0];
        
        for( int y=0; y<output.height; ++y )
        {
            const GRAY* p_input = &input[ {0,y} ];

            for( int x=0; x<output.width; ++x, ++p_output, ++p_input )
            {
                const GRAY center = p_input[ x + radius ];
                
                Vector2 vec;
                for( int i=0; i<direction.size(); ++i )
                {
                    const int dx = i % side;
                    const int dy = i / side;
                    const GRAY target = p_input[ dx + dy*input.width ];
                    
                    vec += direction[i] * ( center.L - target.L );
                }
                
                *p_output = {
                    vec.angle(),
                    (float)core::fast_min( (int)(vec.magnitude() * max_distance_inverse), 255 ), 0 };
            }
        }
        
        return std::move( output );
    }
}

#endif /* GazoShori_hpp */
