#include <iostream>
using namespace std;

#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;
//sides of the rectangle
enum sides{
	left = 1,
	right,
	top,
	bottom
};
class Rect;
static Rect make_rect(int x, int y, int w, int h);
class Rect{
public:
	Rect(const Rect& oRect)
		: x(oRect.x)
		, y(oRect.y)
		, end_x(oRect.end_x)
		, end_y(oRect.end_y)
	{}
	Rect& operator=(const Rect& oRect)
	{
		x = oRect.x;
		y = oRect.y;
		end_x = oRect.end_x;
		end_y = oRect.end_y;
		return *this;
	}
	Rect(int i16PosX, int i16PosY, int i16GetEndX, int i16GetEndY)
		: x(i16PosX), y(i16PosY), end_x(i16GetEndX), end_y(i16GetEndY)
	{}
	Rect()
		: x(0), y(0), end_x(0), end_y(0)
	{}



	bool boIntersect(const Rect& oRect) const
	{
		return (std::min(i16GetEndX(), oRect.i16GetEndX()) - std::max(i16GetPosX(), oRect.i16GetPosX())) > 0
			&& (std::min(i16GetEndY(), oRect.i16GetEndY()) - std::max(i16GetPosY(), oRect.i16GetPosY())) > 0;
	}
	bool boIntersect(const Rect& oRect, Rect& oIntersection) const
	{
		if (boIntersect(oRect))
		{
			oIntersection.vSetPos(std::max(i16GetPosX(), oRect.i16GetPosX()), std::max(i16GetPosY(), oRect.i16GetPosY()));
			oIntersection.vSetEndX(std::min(i16GetEndX(), oRect.i16GetEndX()));
			oIntersection.vSetEndY(std::min(i16GetEndY(), oRect.i16GetEndY()));
			return true;
		}
		return false;
	}

	int i16GetPosX() const
	{
		return x;
	}

	int i16GetPosY() const
	{
		return y;
	}

	int i16GetEndX() const
	{
		return end_x;
	}

	int i16GetEndY() const
	{
		return end_y;
	}

	int u16GetWidth() const
	{
		return end_x - x;
	}

	int u16GetHeight() const
	{
		return end_y - y;
	}

	void vSetPos(int x1, int y1)
	{
		this->x = x1;
		this->y = y1;
	}

	void vSetEndX(int x1)
	{
		this->end_x = x1;
	}

	void vSetEndY(int y1)
	{
		this->end_y = y1;
	}

	void print()
	{
		cout << "{ " << this->x << ", " << this->y << ", " << this->u16GetWidth() << ", " << this->u16GetHeight() << " }" << endl;
	}

	int getArea()
	{
		return this->u16GetWidth() * this->u16GetHeight();
	}

	void printArea()
	{
		cout << "Area = " << getArea() << endl;
	}
	Rect unite(Rect b)
	{
		return Rect(std::min(this->x, b.x), std::min(this->y, b.y), std::max(this->end_x, b.end_x), std::max(this->end_y, b.end_y));
	}

	bool unite_or_not(Rect other, Rect rect_intersecting)
	{
		int a_area, b_area, i_area;
		a_area = this->getArea();
		b_area = other.getArea();
		i_area = rect_intersecting.getArea();
		//ratio of intersecting area to the maximum rect area
		if ((i_area / (std::max(a_area, b_area))) < 0.2){
			return false;
		}
		else
		{
			return true;
		}
	}

	bool unite_or_not_by_total_area(Rect other, Rect rect_intersecting)
	{
		Rect united = this->unite(other);
		float a_area, b_area, u_area, i_area;
		i_area = rect_intersecting.getArea();
		a_area = this->getArea();
		b_area = other.getArea();
		u_area = united.getArea();
		//cout <<"i_area = "<<i_area<<" a_area = "<<a_area<<" b_area = "<<b_area<<" u_area = "<<u_area<<endl;
		int actual_area = a_area + b_area - i_area;
		cout << "actual area = " << actual_area << endl;
		//to calculate the percentage increase (increase/original * 100)
		//float answer = ((u_area - actual_area)/actual_area)*100;
		//cout<<"answer = "<<answer<<endl;
		if ((((u_area - actual_area) / actual_area) * 100) >= 20)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	// if a give rect is inside the reference rect
	bool contains(Rect a) const
	{
		bool isInside = false;
		std::vector<std::pair<int, int>> rect_points;
		rect_points.push_back(std::make_pair(a.x, a.y));
		rect_points.push_back(std::make_pair(a.end_x, a.y));
		rect_points.push_back(std::make_pair(a.x, a.end_y));
		rect_points.push_back(std::make_pair(a.end_x, a.end_y));

		for (std::vector < std::pair<int, int> >::const_iterator it = rect_points.begin(); it != rect_points.end(); ++it)
		{
			int pointX = it->first;
			int pointY = it->second;
			cout << "{ " << pointX << ", " << pointY << "}" << endl;
			if (pointX <= (this->x + (this->u16GetWidth())) && pointX >= (this->x - (this->u16GetWidth())) &&
				pointY <= (this->y + (this->u16GetHeight())) && pointY >= (this->y - (this->u16GetHeight())))
				isInside = true;
			else
				return false;
		} return isInside;

	}

	void split(Rect other, Rect intersection)
	{
		std::vector <Rect> rectlist;

		if (!(this->end_x <= other.end_x))//to ensure the reference rect lies to left of the other rect
		{
			if (this->x > other.x)
			{
				std::swap(*this, other);
				cout << "It is swapped" << endl;
				this->print();
				other.print();
				cout << "**********************************************" << endl;
			}
		}
		if (other.y > this->y)
		{
			if (this->end_y < other.end_y) //if the other rect height extends beyond the reference rect
			{
				Rect newRect = make_rect(intersection.x, this->y, intersection.u16GetWidth(), (this->u16GetHeight() - intersection.u16GetHeight()));
				rectlist.push_back(newRect);
				this->end_x = intersection.x; // reshape the reference rect 
				this->print();
				this->printArea();
				other.print();
				other.printArea();
				newRect.print();
				newRect.printArea();
				cout << "**********************************************" << endl;
			}
			else
			{
				other.x = intersection.end_x; // reshape the other rect
				this->print();
				this->printArea();
				other.print();
				other.printArea();
				cout << "**********************************************" << endl;
			}
		}
		else if (other.y < this->y)
		{
			if (this->end_y > other.end_y)
			{
				Rect newRect = make_rect(intersection.x, intersection.end_y, intersection.u16GetWidth(), (this->u16GetHeight() - intersection.u16GetHeight()));
				this->end_x = intersection.x;
				rectlist.push_back(newRect);
				this->print();
				this->printArea();
				other.print();
				other.printArea();
				newRect.print();
				newRect.printArea();
				cout << "**********************************************" << endl;
			}
			else
			{
				this->end_x = other.x;
				this->print();
				this->printArea();
				other.print();
				other.printArea();
				cout << "**********************************************" << endl;
			}
		}
		else if (other.y == this->y)
		{
			if (this->end_y > other.end_y)
			{
				other.x = this->end_x;
				this->print();
				this->printArea();
				other.print();
				other.printArea();
				cout << "**********************************************" << endl;
			}
			else
			{
				this->end_x = other.x;
				this->print();
				this->printArea();
				other.print();
				other.printArea();
				cout << "**********************************************" << endl;
			}
		}
		else if (this->y == other.y && this->end_y == other.end_y)
		{
			this->end_x = other.x;
		}
	}

	int x;
	int y;
	int end_x;
	int end_y;
};

static Rect make_rect(int x, int y, int w, int h)
{
	return Rect(x, y, x + w, y + h);
}

static Rect unite(Rect a, Rect b)
{
	return Rect(std::min(a.x, b.x), std::min(a.y, b.y), std::max(a.end_x, b.end_x), std::max(a.end_y, b.end_y));
}

int main() {
	Rect rect1 = make_rect(37, 27, 30, 20);
	rect1.print();
	rect1.printArea();
	cout << "**********************************************" << endl;
	Rect rect2 = make_rect(10, 10, 30, 20);
	rect2.print();
	rect2.printArea();
	cout << "**********************************************" << endl;
	Rect rect_intersecting;
	if (rect1.boIntersect(rect2, rect_intersecting))
	{
		rect_intersecting.print();
		rect_intersecting.printArea();
	}
	cout << "**********************************************" << endl;
	Rect unitedRect = unite(rect1, rect2);
	unitedRect.print();
	unitedRect.printArea();
	cout << "**********************************************" << endl;

	if (rect1.unite_or_not_by_total_area(rect2, rect_intersecting)){
		cout << "Can be united" << endl;
		cout << "**********************************************" << endl;
	}
	else{
		cout << "Can be split" << endl;
		cout << "**********************************************" << endl;
		rect1.split(rect2, rect_intersecting);
	}

}