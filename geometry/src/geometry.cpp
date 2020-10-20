#include "geometry.h"
#include <iostream>
#include <math.h>
#include <set>
#include <typeinfo>

using namespace task;

Point::Point(double _x, double _y) : x(_x), y(_y){};

bool Point::operator==(const Point &other_point) const {
  return (this->x == other_point.x) && (this->y == other_point.y);
}

bool Point::operator!=(const Point &other_point) const {
  return !(*this == other_point);
}

Line::Line(double _shift, double _slope) : shift(_shift), slope(_slope){};

Line::Line(const Point &p1, const Point &p2)
    : slope((p2.y - p1.y) / (p2.x - p1.x)),
      shift(p1.y - (p2.y - p1.y) / (p2.x - p1.x) * p1.x){};

Line::Line(const Point &p, double _slope)
    : slope(_slope), shift(p.y - _slope * p.x){};

bool Line::operator==(const Line &other_line) const {
  return (this->shift == other_line.shift) && (this->slope == other_line.slope);
}

bool Line::operator!=(const Line &other_line) const {
  return !((this->shift == other_line.shift) &&
           (this->slope == other_line.slope));
}

Shape::~Shape(){};

Polygon::Polygon(const std::vector<Point> &vec_pks) : vector_peaks(vec_pks){};

Polygon::~Polygon(){};

size_t Polygon::verticesCount() const { return this->vector_peaks.size(); };

const std::vector<Point> Polygon::getVertices() const {
  std::vector<Point> res_vec(this->vector_peaks);

  return res_vec;
}

double getDistance(const Point &p1, const Point &p2) {
  double dist = 0;

  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

double Polygon::perimeter() const {
  double per = 0;
  if (this->vector_peaks.size() == 0)
    return 0;

  Point prev_pt(this->vector_peaks[0]);
  for (size_t i = 1; i < this->vector_peaks.size(); ++i) {
    Point cur_pt(this->vector_peaks[i]);
    per += getDistance(prev_pt, cur_pt);
    prev_pt = cur_pt;
  }

  per += getDistance(prev_pt, this->vector_peaks[0]);

  return per;
}

double Polygon::area() const {
  double ar = 0;
  if (this->vector_peaks.size() == 0)
    return 0;

  Point prev_pt(this->vector_peaks[0]);
  for (size_t i = 1; i < this->vector_peaks.size(); ++i) {
    Point cur_pt(this->vector_peaks[i]);
    ar += prev_pt.x * cur_pt.y;
    ar -= prev_pt.y * cur_pt.x;
    prev_pt = cur_pt;
  }

  ar += prev_pt.x * this->vector_peaks[0].y;
  ar -= prev_pt.y * this->vector_peaks[0].x;

  return (ar > 0) ? (ar / 2) : (-ar / 2);
};

struct LineTgAngleLine {

public:
  double l1_len;
  double l2_len;
  double tg_angl;

  LineTgAngleLine() : l1_len(0.), l2_len(0.), tg_angl(0.){};
  ~LineTgAngleLine(){};

  bool operator==(const LineTgAngleLine &another) const {
    if (this->l1_len == another.l1_len && this->l2_len == another.l2_len &&
        this->tg_angl == another.tg_angl)
      return true;

    return false;
  }

  bool operator<(const LineTgAngleLine &another) const {
    if (this->tg_angl < another.tg_angl)
      return true;

    return false;
  }
};

std::set<LineTgAngleLine>
getVectorLineAngleLine(const std::vector<Point> &vec_pks) {
  std::set<LineTgAngleLine> set_lal;
  LineTgAngleLine tmp_lal, rev_tmp_lal;
  for (size_t i = 1; i < vec_pks.size() - 1; ++i) {
    Point prev_pt(vec_pks[i - 1]);
    Point cur_pt(vec_pks[i]);
    Point next_pt(vec_pks[i + 1]);

    Line tmp_l1(prev_pt, cur_pt);
    Line tmp_l2(cur_pt, next_pt);
    tmp_lal.l1_len = getDistance(prev_pt, cur_pt);
    tmp_lal.l2_len = getDistance(cur_pt, next_pt);
    tmp_lal.tg_angl =
        (tmp_l2.slope - tmp_l1.slope) / (1 - tmp_l1.slope * tmp_l2.slope);

    rev_tmp_lal.l1_len = tmp_lal.l2_len;
    rev_tmp_lal.l2_len = tmp_lal.l1_len;
    rev_tmp_lal.tg_angl = -tmp_lal.tg_angl;

    set_lal.insert(tmp_lal);
    set_lal.insert(rev_tmp_lal);
  }

  Point prev_pt(vec_pks[vec_pks.size() - 1]);
  Point cur_pt(vec_pks[0]);
  Point next_pt(vec_pks[1]);

  Line tmp_l1(prev_pt, cur_pt);
  Line tmp_l2(cur_pt, next_pt);
  tmp_lal.l1_len = getDistance(prev_pt, cur_pt);
  tmp_lal.l2_len = getDistance(cur_pt, next_pt);
  tmp_lal.tg_angl =
      (tmp_l2.slope - tmp_l1.slope) / (1 - tmp_l1.slope * tmp_l2.slope);

  rev_tmp_lal.l1_len = tmp_lal.l2_len;
  rev_tmp_lal.l2_len = tmp_lal.l1_len;
  rev_tmp_lal.tg_angl = -tmp_lal.tg_angl;

  set_lal.insert(tmp_lal);
  set_lal.insert(rev_tmp_lal);

  return set_lal;
}

bool Polygon::operator==(const Shape &another) const {
  if (typeid(*this) == typeid(another)) {
    std::vector<Point> vek_pks_another(
        dynamic_cast<const Polygon &>(another).vector_peaks);
    std::set<LineTgAngleLine> set_this(getVectorLineAngleLine(vek_pks_another));
    std::set<LineTgAngleLine> set_another(
        getVectorLineAngleLine(this->vector_peaks));

    if (set_this == set_another)
      return true;
  }

  return false;
}

bool Polygon::operator!=(const Shape &another) const {
  return !((*this) == another);
}

void Polygon::rotate(Point center, double angle) {
  for (size_t i = 0; i < this->vector_peaks.size(); ++i) {
    Point cur_pt = this->vector_peaks[i];
    Point new_pt(0, 0);

    new_pt.x = (cur_pt.x - center.x) * cos(angle) -
               (cur_pt.y - center.y) * sin(angle) + center.x;
    new_pt.y = (cur_pt.x - center.x) * sin(angle) +
               (cur_pt.y - center.y) * cos(angle) + center.y;

    this->vector_peaks[i] = new_pt;
  }
}

Point scalePoint(const Point &p0, const Point &p, double coef) {
  Point new_p(0, 0);

  double dist = getDistance(p, p0);
  double val_cos = (p.x - p0.x) / dist;
  double val_sin = (p.y - p0.y) / dist;

  double new_dist = coef * dist;
  new_p.x = p0.x + new_dist * val_cos;
  new_p.y = p0.y + new_dist * val_sin;

  return new_p;
}

void Polygon::scale(Point center, double coefficient) {
  for (size_t i = 0; i < this->vector_peaks.size(); ++i)
    this->vector_peaks[i] =
        scalePoint(center, this->vector_peaks[i], coefficient);
}

void Polygon::reflex(Point center) { this->scale(center, -1); }

void Polygon::reflex(Line axis) {
  for (size_t i = 0; i < this->vector_peaks.size(); ++i) {
    Point pt_on_line(0., 0.);
    Point cur_pt(this->vector_peaks[i]);

    pt_on_line.x = (cur_pt.x - axis.slope * (axis.shift - cur_pt.y)) /
                   (1 + pow(axis.slope, 2));
    pt_on_line.y = axis.slope * pt_on_line.x + axis.shift;

    this->vector_peaks[i] = scalePoint(pt_on_line, this->vector_peaks[i], -1);
  }
}

Triangle::Triangle(const std::vector<Point> &vec_pks) : Polygon(vec_pks){};
Triangle::Triangle(const Point &p1, const Point &p2, const Point &p3)
    : Polygon({p1, p2, p3}){};
Triangle::~Triangle(){};

Circle Triangle::circumscribedCircle() const {
  double a, b, c;

  a = getDistance(this->getVertices()[0], this->getVertices()[1]);
  b = getDistance(this->getVertices()[1], this->getVertices()[2]);
  c = getDistance(this->getVertices()[2], this->getVertices()[0]);

  Point center_a((this->getVertices()[0].x + this->getVertices()[1].x) / 2,
                 (this->getVertices()[0].y + this->getVertices()[1].y) / 2);

  Point center_b((this->getVertices()[1].x + this->getVertices()[2].x) / 2,
                 (this->getVertices()[1].y + this->getVertices()[2].y) / 2);

  double rad, x_cntr, y_cntr;
  rad = a * b * c / this->area() / 4;

  Line a_line(this->getVertices()[0], this->getVertices()[1]);
  Line b_line(this->getVertices()[1], this->getVertices()[2]);

  x_cntr = (a_line.slope * b_line.slope * (center_b.y - center_a.y) +
            a_line.slope * center_b.x - b_line.slope * center_a.x) /
           (a_line.slope - b_line.slope);
  y_cntr = center_a.y - (x_cntr - center_a.x) / a_line.slope;

  Point centre_circumscribed_circle(x_cntr, y_cntr);

  return Circle(centre_circumscribed_circle, rad);
}

Circle Triangle::inscribedCircle() const {
  double a, b, c;

  a = getDistance(this->getVertices()[0], this->getVertices()[1]);
  b = getDistance(this->getVertices()[1], this->getVertices()[2]);
  c = getDistance(this->getVertices()[2], this->getVertices()[0]);

  double rad = this->area() / this->perimeter();

  Line a_line(this->getVertices()[0], this->getVertices()[1]);
  Line b_line(this->getVertices()[1], this->getVertices()[2]);
  Line c_line(this->getVertices()[2], this->getVertices()[0]);

  double rel1, rel2;
  rel1 = a / b;
  rel2 = b / c;

  Point p_bis_on_c(
      (this->getVertices()[0].x + rel1 * this->getVertices()[2].x) / (1 + rel1),
      (this->getVertices()[0].y + rel1 * this->getVertices()[2].y) /
          (1 + rel1));

  Point p_bis_on_a(
      (this->getVertices()[1].x + rel2 * this->getVertices()[0].x) / (1 + rel2),
      (this->getVertices()[1].y + rel2 * this->getVertices()[0].y) /
          (1 + rel2));

  Line bis_on_a(this->getVertices()[2], p_bis_on_a);
  Line bis_on_c(this->getVertices()[1], p_bis_on_c);

  double x_cntr, y_cntr;

  x_cntr =
      (bis_on_c.shift - bis_on_a.shift) / (bis_on_a.slope - bis_on_c.slope);
  y_cntr = bis_on_a.slope * x_cntr + bis_on_a.shift;

  Point centre_inscribed_circle(x_cntr, y_cntr);

  return Circle(centre_inscribed_circle, rad);
}

/// does not work correctly
std::vector<Point> Rectangle::set_peaks(const Point &p1, const Point &p3,
                                        double rel) {
  double dist = getDistance(p1, p3);

  double len_first_side, len_second_side;
  len_first_side = dist / sqrt(1 + rel * rel);
  len_second_side = dist / sqrt(1 + 1 / (rel * rel));

  double max_side, min_side;
  min_side = std::min(len_first_side, len_second_side);
  max_side = std::max(len_first_side, len_second_side);

  Point centre_rec((p1.x + p3.x) / 2, (p1.y + p3.y) / 2);
  Line first_diag_line(p1, p3);

  double angle_sec_diag, _tg;
  _tg = min_side / max_side;
  angle_sec_diag = (_tg + _tg) / (1 - _tg * _tg);

  Line second_diag_line(centre_rec, first_diag_line.slope - angle_sec_diag);

  double a, b, c;
  a = pow(second_diag_line.slope, 2) + 1;
  b = second_diag_line.slope * (centre_rec.y - second_diag_line.shift) + 1;
  c = pow(centre_rec.x, 2) + pow((centre_rec.y - second_diag_line.shift), 2) -
      dist * dist / 4;

  double x1, x2;
  x1 = (-b + sqrt(b * b - a * c)) / a;
  x2 = (-b - sqrt(b * b - a * c)) / a;

  double y1, y2;
  y1 = second_diag_line.slope * x1 + second_diag_line.shift;
  y2 = second_diag_line.slope * x2 + second_diag_line.shift;

  // std::cout << x1 << " " << y1 << std::endl; /// nan
  // std::cout << x2 << " " << y2 << std::endl; /// nan

  std::vector<Point> contr_vec({p1, p3});
  return contr_vec;
}

Rectangle::Rectangle(const Point &p1, const Point &p3, double rel)
    : Polygon(set_peaks(p1, p3, rel)), relation(rel){};
Rectangle::~Rectangle(){};

Ellipse::Ellipse(const Point &_f1, const Point &_f2, double _dist)
    : f1(_f1), f2(_f2), dist(_dist){};
Ellipse::~Ellipse(){};

std::pair<Point, Point> Ellipse::focuses() const {
  std::pair<Point, Point> _focuses({this->f1, this->f2});
  return _focuses;
};

double Ellipse::eccentricity() const {
  double minor_semiaxis, major_semiaxis, half_dist_between_foc;

  major_semiaxis = this->dist / 2;
  half_dist_between_foc = getDistance(f1, f2) / 2;
  minor_semiaxis = sqrt(pow(major_semiaxis, 2) - pow(half_dist_between_foc, 2));

  return half_dist_between_foc / major_semiaxis;
}

Point Ellipse::center() const {
  return Point((this->f1.x + this->f2.x) / 2, (this->f1.y + this->f2.y) / 2);
}

double Ellipse::perimeter() const {
  double minor_semiaxis, major_semiaxis, half_dist_between_foc;

  major_semiaxis = this->dist / 2;
  half_dist_between_foc = getDistance(f1, f2) / 2;
  minor_semiaxis = sqrt(pow(major_semiaxis, 2) - pow(half_dist_between_foc, 2));

  return 4 * major_semiaxis *
         std::comp_ellint_2(half_dist_between_foc / major_semiaxis);
};

double Ellipse::area() const {
  double minor_semiaxis, major_semiaxis, half_dist_between_foc;

  major_semiaxis = this->dist / 2;
  half_dist_between_foc = getDistance(f1, f2) / 2;
  minor_semiaxis = sqrt(pow(major_semiaxis, 2) - pow(half_dist_between_foc, 2));

  return M_PI * major_semiaxis * minor_semiaxis;
};

bool Ellipse::operator==(const Shape &another) const {
  if (typeid(*this) == typeid(another)) {
    Ellipse tmp_el = dynamic_cast<const Ellipse &>(another);
    if (this->dist == tmp_el.dist &&
        getDistance(this->f1, this->f2) == getDistance(tmp_el.f1, tmp_el.f2))
      return true;
  }

  return false;
};

bool Ellipse::operator!=(const Shape &another) const {
  return !((*this) == another);
};

void Ellipse::rotate(Point center, double angle){};
void Ellipse::scale(Point center, double coefficient){};
void Ellipse::reflex(Point center){};
void Ellipse::reflex(Line axis){};

Circle::Circle(const Point &cntr, double _rad)
    : Ellipse(cntr, cntr, _rad), point_center(cntr), rad(_rad){};
Circle::~Circle(){};

double Circle::radius() const { return this->rad; }
