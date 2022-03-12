#include <iostream>

constexpr int ScreenWidth = 10000;
constexpr int ScreenHeight = 10000;

constexpr double XLowerBound = -2.0;
constexpr double XUpperBound = 0.5;
constexpr double YLowerBound = -1.3;
constexpr double YUpperBound = 1.3;

constexpr double XViewportSize = XUpperBound - XLowerBound;
constexpr double YViewportSize = YUpperBound - YLowerBound;

constexpr int MaxIterations = 1000;
constexpr int PixelAmount = ScreenWidth * ScreenHeight;

struct Point {
    double x;
    double y;
};

Point map_pixel_to_viewport(Point point) {
    return Point{
        (XViewportSize / ScreenWidth) * point.x + XLowerBound,
        (YViewportSize / ScreenHeight) * point.y + YLowerBound,
    };
}

bool inside_mandelbrot(Point point) {
    return (point.x * point.x + point.y * point.y) <= 4;
}

Point iterate_mandelbrot_point(Point point, Point& initial) {
    return Point{
        point.x * point.x - point.y * point.y + initial.x,
        2 * point.x * point.y + initial.y
    };
}

void print_pixels(uint8_t pixels[PixelAmount]) {
    std::cout << "P5\n" << ScreenWidth << ' ' << ScreenHeight << "\n255\n";
    
    for (size_t i = 0; i < PixelAmount; i++)
        std::cout << pixels[i];
}

double calculateHue(uint32_t pixelsIntegral) {
    double x = (double)pixelsIntegral / PixelAmount;
    return (x - x * x) * 255 / 0.25;
}

uint32_t iterationsPerPixel[PixelAmount];
uint32_t pixelsPerIteration[MaxIterations + 1];
uint8_t pixels[PixelAmount];

int main() {
    for (size_t y = 0; y < ScreenHeight; y++) {
        for (size_t x = 0; x < ScreenWidth; x++) {
            
            Point initial = map_pixel_to_viewport(Point{(double)x, (double)y});
            Point current = Point{0, 0};
            size_t it;
            
            for (it = 0; inside_mandelbrot(current) && it < MaxIterations; it++)
                current = iterate_mandelbrot_point(current, initial);

            pixelsPerIteration[it]++;
            iterationsPerPixel[y * ScreenHeight + x] = it;
        }
    }

    for (size_t y = 0; y < ScreenHeight; y++) {
        for (size_t x = 0; x < ScreenWidth; x++) {
            auto iterations = iterationsPerPixel[y * ScreenHeight + x];
            auto pixelsIntegral = 0;
            
            for (size_t i = 0; i <= iterations; i++)
                pixelsIntegral += pixelsPerIteration[i];
            
            pixels[y * ScreenHeight + x] = calculateHue(pixelsIntegral);
        }
    }
    
    print_pixels(pixels);
}