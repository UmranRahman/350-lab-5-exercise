#include <iostream>
#include <optional>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;

using Point2D = sf::Vector2f;

Point2D lerp(Point2D a, Point2D b, float t) {
    return (1 - t) * a + t * b;
}

// TODO: (Part 1) Define a function that samples a cubic Bezier curve at t in [0, 1].
Point2D getPoint(const std::vector<sf::Vector2f>& pts, float t) { 
    
    Point2D a = lerp(pts[0], pts[1], t);
    Point2D b = lerp(pts[1], pts[2], t);
    Point2D c = lerp(pts[2], pts[3], t);

    Point2D d = lerp(a, b, t);
    Point2D e = lerp(b, c, t);

    return lerp(d, e, t); 
}

// TODO: (Part 2) Define a function that returns the curve's slope at t in [0, 1].
Point2D getSlope(const std::vector<sf::Vector2f>& pts, float t) {
    // AI-assisted:
    float u = 1 - t;
    Point2D s1 = 3.f*u*u*(pts[1]-pts[0]);
    Point2D s2 = 6.f*u*t*(pts[2]-pts[1]);
    Point2D s3 = 3.f*t*t*(pts[3]-pts[2]);
    Point2D slope = s1 + s2 + s3;
    return slope; 
}

// TODO: (Part 1) Store four control points for the curve.
// TODO: (Part 2) Track animation time for the square moving along the curve.
// TODO: (Part 3) Track the index of the control point being dragged.

// AI-assisted: From Claude code prompt : global state for points, frame counter, and drag index (Claude, prompt 2)
std::vector<sf::Vector2f> pts = {{100,600},{200,100},{500,100},{700,600}};
int frame = 0;
int editingIndex = -1;   // -1 = not dragging
int FRAMES_PER_LOOP = 90;

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            // TODO: (Part 3) On left-click, select the closest control point
            // using mouse->position and start dragging it.
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
            // TODO: (Part 3) On left-button release, stop dragging.
        } else if (const auto* mouse = event->getIf<sf::Event::MouseMoved>()) {
            // TODO: (Part 3) Move the selected control point to mouse->position.
            // TODO: (Part 4) Maintain matching slopes at shared endpoints.
            // When moving point 3, move point 5 without changing its distance
            // from point 4 (point numbers here start at 1).
        } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            // TODO: (Part 4) '+' adds three control points; '-' removes three,
            // keeping at least four points.
        }
    }
}

void render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Part 1) Sample GetPoint over t in [0, 1] and connect samples using the line-drawing
    // code from your project. Draw all four control points as circles after drawing the curve.
    // ====== ====== ======
    sf::VertexArray curve(sf::PrimitiveType::LineStrip);
    for (int i = 0; i <= 100; i++){
        float t = i / 100.0f;
        Point2D p = getPoint(pts, t);
        curve.append(sf::Vertex{p, sf::Color::Green});
    }
    window.draw(curve);

    for (const auto&p : pts){
        sf::CircleShape dot(5.0f);
        sf::Vector2f position(p);
        sf::Vector2f origin(5.0f,5.0f);
        dot.setOrigin(origin);
        dot.setPosition(position);
        dot.setFillColor(sf::Color::Yellow);
        window.draw(dot);
    }

    // ====== ====== ======
    // TODO: (Part 2) Draw a small square moving repeatedly along the curve.
    // Use GetSlope to orient it to the curve at each time step.
    // ====== ====== ======
    float t = float(float(frame%FRAMES_PER_LOOP)/FRAMES_PER_LOOP);
    frame += 1;

    //Draw Square
    Point2D position = getPoint(pts, t);
    sf::RectangleShape rect({16.0f, 16.0f});
    rect.setOrigin({8.0f, 8.0f});
    rect.setPosition(position);
    rect.setFillColor(sf::Color::Red);
    

    Point2D slope = getSlope(pts, t);
    //AI Assistant: Claude
    rect.setRotation(sf::radians(std::atan2(slope.y, slope.x)));
    window.draw(rect);

    // ====== ====== ======
    // TODO: (Part 3) Draw control handles from point 1 to 2 and point 3 to 4.
    // TODO: (Part 4) Draw all connected cubic Bezier segments and their handles.
    // ====== ====== ======

    // ====== ====== ======
    // TODO: (Bonus) Support multiple curves, a Galaga screen overlay at a 1:2 ratio, and exporting
    // curve points as C++ code for Project 1b.
    // ====== ====== ======

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Bezier Curve Editor");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
