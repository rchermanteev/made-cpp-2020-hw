#pragma once

#include <vector>

namespace task {

struct Point {

public:
  double x, y;

  Point(double _x, double _y);

  bool operator==(const Point &other_point) const;
  bool operator!=(const Point &other_point) const;
};

class Line {

public:
  double shift, slope;

  Line(double _shift, double _slope);
  Line(const Point &p1, const Point &p2);
  Line(const Point &p, double _slope);

  bool operator==(const Line &other_line) const;
  bool operator!=(const Line &other_line) const;
};

class Shape {

public:
  virtual double perimeter() const = 0;
  virtual double area() const = 0;
  virtual bool operator==(const Shape &another) const = 0;
  virtual bool operator!=(const Shape &another) const = 0;

  virtual void rotate(Point center, double angle) = 0;
  virtual void scale(Point center, double coefficient) = 0;
  virtual void reflex(Point center) = 0;
  virtual void reflex(Line axis) = 0;

  virtual ~Shape() = 0;
};

class Polygon : public Shape {
private:
  std::vector<Point> vector_peaks;

public:
  Polygon(const std::vector<Point> &vec_pks);
  ~Polygon();

  size_t verticesCount() const;
  const std::vector<Point> getVertices() const;

  double perimeter() const override;
  double area() const override;
  bool operator==(const Shape &another) const override;
  bool operator!=(const Shape &another) const override;

  void rotate(Point center, double angle) override;
  void scale(Point center, double coefficient) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
};

class Rectangle : public Polygon {
private:
  double relation;

  static std::vector<Point> set_peaks(const Point &p1, const Point &p3,
                                      double rel);

public:
  explicit Rectangle(const Point &p1, const Point &p3, double rel);
  ~Rectangle();
};

class Ellipse : public Shape {
private:
  Point f1;
  Point f2;
  double dist;

public:
  Ellipse(const Point &_f1, const Point &_f2, double dist);
  ~Ellipse();

  std::pair<Point, Point> focuses() const;
  double eccentricity() const;
  Point center() const;

  double perimeter() const override;
  double area() const override;
  bool operator==(const Shape &another) const override;
  bool operator!=(const Shape &another) const override;

  void rotate(Point center, double angle) override;
  void scale(Point center, double coefficient) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
};

class Circle : public Ellipse {
private:
  Point point_center;
  double rad;

public:
  Circle(const Point &cntr, double _rad);
  ~Circle();

  double radius() const;
};

class Triangle : public Polygon {
public:
  Triangle(const std::vector<Point> &vec_pks);
  Triangle(const Point &p1, const Point &p2, const Point &p3);
  ~Triangle();

  Circle circumscribedCircle() const;
  Circle inscribedCircle() const;
};

} // namespace task