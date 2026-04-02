#include <math.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "color.h"
#include "light.h"
#include "object.h"
#include "ray.h"
#include "vec3.h"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

Color ray_color(const Ray& r) {
    Vec3 unit_direction = r.direction().normalized();
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

Vec3 readVector(std::istream& inFile) {
    double x;
    double y;
    double z;
    std::string string;
    inFile >> string;
    // std::cerr << string << '\n';
    inFile >> x >> y >> z;
    return Vec3(x, y, z);
}

double readValue(std::istream& inFile) {
    std::string str;
    inFile >> str;
    double value;
    inFile >> value;
    return value;
}

unique_ptr<Object> readObject(std::istream& inFile, string objectType) {
    Vec3 center = readVector(inFile);
    double r = readValue(inFile);
    double kd = readValue(inFile);
    double ks = readValue(inFile);
    double ka = readValue(inFile);
    Vec3 od = readVector(inFile);
    Vec3 os = readVector(inFile);
    double kgls = readValue(inFile);
    double refl = readValue(inFile);
    return make_unique<Sphere>(center, r, od, os, kd, ks, ka, kgls, refl);
}

unique_ptr<Light> readLight(std::istream& inFile, string lightType) {
    Point3 position = readVector(inFile);
    Color color = readVector(inFile);
    return make_unique<PointLight>(position, color);
}

Color getAmbientIllumination(const Ray& ray, const shared_ptr<Object>& object, const Color& ambientLight) {
    // return Color(0, 0, 0);
    return object->ka * ambientLight * object->Od;
}

Vec3 getReflection(const Vec3& directionToLight, const Vec3& normal) {
    return (2 * normal * (directionToLight.dot(normal))) - directionToLight;
}

Color getDiffuseIllumination(const Ray& ray, const shared_ptr<Object>& object, const Vec3& intersection, const std::vector<shared_ptr<Light>>& lights) {
    // return Color(0, 0, 0);
    Color totalDiffuse = Color(0, 0, 0);

    auto normal = object->surface_normal(intersection);

    for (auto& light : lights) {
        double dot_result = normal.dot(light->getDirectionToLight(intersection));
        if (dot_result < 0) {
            dot_result = 0;
        }
        totalDiffuse += object->kd * light->color * object->Od * dot_result;
    }

    return totalDiffuse;
}

Color getSpecularIllumination(const Ray& ray, const shared_ptr<Object>& object, const Vec3& intersection, const std::vector<shared_ptr<Light>>& lights) {
    // return Color(0, 0, 0);
    Color totalSpecular(0, 0, 0);
    auto normal = object->surface_normal(intersection);

    for (auto& light : lights) {
        auto reflection = getReflection(light->getDirectionToLight(intersection), normal);
        double dot_result = (-ray.direction()).dot(reflection);
        if (dot_result < 0) {
            dot_result = 0.0;
        }
        totalSpecular += object->ks * light->color * object->Os * std::pow(dot_result, object->kgls);
    }

    return totalSpecular;
}

Color getIllumination(const Ray& ray, const shared_ptr<Object>& object, const std::vector<shared_ptr<Light>>& lights, const Color& ambientLight, const Vec3& intersection) {
    return getAmbientIllumination(ray, object, ambientLight) +
           getDiffuseIllumination(ray, object, intersection, lights) +
           getSpecularIllumination(ray, object, intersection, lights);
}

int main(int argc, char* argv[]) {
    string inFileName = "input.txt";
    string outFileName = "output.ppm";

    if (argc == 1) {
        std::cout << "Enter input file name: ";
        std::cin >> inFileName;
        std::cout << '\n';
    }
    if (argc == 2) {
        inFileName = std::string(argv[1]);
        std::cout << "Output file name defaulting to output.ppm\n";
    }
    if (argc == 3) {
        inFileName = std::string(argv[1]);
        outFileName = std::string(argv[2]);
    }
    if (argc > 3) {
        throw std::runtime_error("Too many arguments! Enter the input file name and then the output file name\n");
    }

    std::ifstream inFile(inFileName);
    std::ofstream outFile(outFileName);

    if (!inFile) {
        std::cerr << "Input file failed to open\n";
        return 1;
    }
    if (!outFile) {
        std::cerr << "Output file failed to open\n";
        return 2;
    }

    auto lookAt = readVector(inFile);
    auto lookFrom = readVector(inFile);
    auto lookUp = readVector(inFile);

    double fov = (readValue(inFile) / 180.0) * M_PI;

    auto directionToLight = readVector(inFile);
    Color lightColor = readVector(inFile);
    Color ambientLight = readVector(inFile);
    Color backgroundColor = readVector(inFile);

    std::vector<shared_ptr<Object>> objects;
    std::vector<shared_ptr<Light>> lights;

    shared_ptr<Light> directionLight = make_shared<DirectionLight>(directionToLight, lightColor);
    lights.push_back(directionLight);

    string input;
    while (inFile >> input) {
        if (input == "#") {
            string line;
            std::getline(inFile, line);
        } else if (input == "Sphere") {
            objects.push_back(readObject(inFile, input));
        } else if (input == "PointLight") {
            lights.push_back(readLight(inFile, input));
        } else {
            throw(std::runtime_error("Something is wrong in the input file."));
        }
    }

    double aspect_ratio = 1.0 / 1.0;

    int image_width = 800;  // THIS IS THE IMAGE WIDTH

    int image_height = int(image_width / aspect_ratio);
    if (image_height < 1) {
        image_height = 1;
    }

    double viewport_height = 2 * std::tan(fov / 2);
    double viewport_width = viewport_height * (double(image_width) / double(image_height));

    auto w = (lookFrom - lookAt).normalized();
    auto u = (lookUp.cross(w)).normalized();
    auto v = (w.cross(u)).normalized();

    Vec3 viewport_u = u * viewport_width;
    Vec3 viewport_v = -v * viewport_height;

    Vec3 pixel_delta_u = viewport_u / image_width;
    Vec3 pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = ((lookFrom - w) - viewport_u / 2) - viewport_v / 2;

    auto pixel00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Create the headers before the image itself
    outFile << "P3\n";
    outFile << image_width << " " << image_height << "\n";
    outFile << "255\n";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_location + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - lookFrom;
            Ray ray(lookFrom, ray_direction);

            double distance = 1000000000000.0;
            shared_ptr<Object> closestObject = nullptr;
            unique_ptr<Vec3> intersection = nullptr;

            // Find the closest object in this path.
            for (auto& object : objects) {
                intersection = object->getIntersection(ray);
                if (intersection) {
                    double length = (*intersection - lookFrom).length_squared();
                    if (length < distance) {
                        distance = length;
                        closestObject = object;
                    }
                }
            }
            if (closestObject) {
                intersection = closestObject->getIntersection(ray);
            }
            if (!intersection) {
                print_color(outFile, backgroundColor);
            } else {
                Color pixelColor = getIllumination(ray, closestObject, lights, ambientLight, *intersection);
                Color pixelColorClamped = pixelColor.clamp(0, 1);
                print_color(outFile, pixelColorClamped);
            }
        }
    }

    return 0;
}