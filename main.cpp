#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>

using namespace std::literals;
using namespace svg;

namespace shapes {

class Triangle : public svg::Drawable {
public:
  Triangle(svg::Point p1, svg::Point p2, svg::Point p3) : p1_(p1)  , p2_(p2)  , p3_(p3)  {}

    // Реализует метод Draw интерфейса svg::Drawable
  void Draw(svg::ObjectContainer& container) const override {
      container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
  }
virtual ~Triangle() =default;
private:
  svg::Point p1_, p2_, p3_;
};

class Star : public svg::Drawable {

public:
  Star(Point center, double outer_rad, double inner_rad, int num_rays) :
             scentr_(center),souter_rad_(outer_rad),sinner_rad_(inner_rad),
             snum_rays_(num_rays){}
    void Draw(svg::ObjectContainer& container) const override {
          container.Add(CreateStar(scentr_,souter_rad_,sinner_rad_,snum_rays_));
    }
    virtual ~Star() = default;
private:
  Point scentr_;
  double souter_rad_;
  double sinner_rad_;
  int snum_rays_;
  Polyline CreateStar(Point center, double outer_rad, double inner_rad, int num_rays) const {
    Polyline polyline;
    for (int i = 0; i <= num_rays; ++i) {
      double angle = 2 * M_PI * (i % num_rays) / num_rays;
      polyline.AddPoint({center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle)});
      if (i == num_rays) {
        break;
      }
      angle += M_PI / num_rays;
      polyline.AddPoint({center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle)});
    }
      return polyline;
  }


};

class Snowman : public svg::Drawable {
  public:
    Snowman(svg::Point p, double r) : centr_(p),radius_(r){}
    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer& container) const override {

         svg::Circle cir1;
         cir1.SetCenter(Point{centr_.x,(centr_.y+radius_*5.0)});
         cir1.SetRadius(2.0*radius_);
         container.Add(cir1);

         cir1.SetCenter(Point{centr_.x,(centr_.y+radius_*2.0)});
         cir1.SetRadius(1.5*radius_);
         container.Add(cir1);

         cir1.SetCenter(centr_);
         cir1.SetRadius(radius_);
         container.Add(cir1);



    }
    virtual ~Snowman() = default;
  private:
    Point centr_;
    double radius_;
};

} // namespace shapes

template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}
int main() {
    using namespace svg;
        using namespace shapes;
        using namespace std;

        vector<unique_ptr<svg::Drawable>> picture;

        picture.emplace_back(make_unique<Triangle>(Point{100, 20}, Point{120, 50}, Point{80, 40}));
        // 5-лучевая звезда с центром {50, 20}, длиной лучей 10 и внутренним радиусом 4
        picture.emplace_back(make_unique<Star>(Point{50.0, 20.0}, 10.0, 4.0, 5));
        // Снеговик с "головой" радиусом 10, имеющей центр в точке {30, 20}
        picture.emplace_back(make_unique<Snowman>(Point{30, 20}, 10.0));

        svg::Document doc;
        // Так как документ реализует интерфейс ObjectContainer,
        // его можно передать в DrawPicture в качестве цели для рисования
        DrawPicture(picture, doc);

        // Выводим полученный документ в stdout
        doc.Render(cout);
}
