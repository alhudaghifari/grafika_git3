#include "drawer.h"

Drawer::Drawer(Canvas* c) {
	destination = c;

  xOffset = 0;
  yOffset = 0;
  xClipWidth = MAX_CANVAS_WIDTH;
  yClipHeight = MAX_CANVAS_HEIGHT;

  xTranslate = 0;
  yTranslate = 0;
  drawScale = 1;
}

void Drawer::draw_shapes(std::vector<Shape*> shapes) {
	for (size_t i = 0; i < shapes.size(); i++) {
		draw_shape(shapes[i]);
	}
}

void Drawer::draw_shape(Shape *S){
	int num_titik = S->getSize();
	for(int i = 0;i < num_titik - 1; ++i){
		gambarGaris(
			(S->getPoint(i)).translated(xTranslate, yTranslate).scaleUp(drawScale),
			(S->getPoint(i+1)).translated(xTranslate, yTranslate).scaleUp(drawScale),
			drawScale > 1 ? drawScale-1 : 0,
			S->color
		);
	}
}


void Drawer::draw_circles(std::vector<Circle*> circles) {
	for (size_t i = 0; i < circles.size(); i++) {
		draw_circle(circles[i]);
	}
}

void Drawer::draw_circle(Circle* C) {
	int d, p, q;

    p = 0;
    q = C->getRadius();
    d = 3 - 2*C->getRadius();

    drawEightPoint(C->getCenter(), p, q,C->color);

    while (p < q) {
        p++;
        if (d<0) {
            d = d + 4*p + 6;
        }
        else {
            q--;
            d = d + 4*(p-q) + 10;
        }

        drawEightPoint(C->getCenter(), p, q,C->color);
    }
}

void Drawer::drawEightPoint(Point P, int p, int q, Color C) {
	Point temp;
	temp.setX(P.getX()+p); temp.setY(P.getY()+q);
	gambarGaris(P,temp,1,C);

	temp.setX(P.getX()-p); temp.setY(P.getY()+q);
	gambarGaris(P,temp,1,C);
	
	temp.setX(P.getX()+p); temp.setY(P.getY()-q);
	gambarGaris(P,temp,1,C);
	
	temp.setX(P.getX()-p); temp.setY(P.getY()-q);
	gambarGaris(P,temp,1,C);

	temp.setX(P.getX()+q); temp.setY(P.getY()+p);
	gambarGaris(P,temp,1,C);

	temp.setX(P.getX()-q); temp.setY(P.getY()+p);
	gambarGaris(P,temp,1,C);
	
	temp.setX(P.getX()+q); temp.setY(P.getY()-p);
	gambarGaris(P,temp,1,C);
	
	temp.setX(P.getX()-q); temp.setY(P.getY()-p);
	gambarGaris(P,temp,1,C);
}

void Drawer::gambarGaris(Point P1, Point P2, int tebal, Color color) {
	if (!lineInsideRect(P1, P2, 0, 0, xClipWidth, yClipHeight)) return;

	for(int t1 = -tebal; t1 <= tebal; ++t1){
		for(int t2 = -tebal; t2 <= tebal; ++t2){
			int xawal = P1.getX() < P2.getX() ? P1.getX() : P2.getX();
			int xakhir = P2.getX() > P1.getX() ? P2.getX() : P1.getX();

			int yawal = P1.getY(), yakhir = P2.getY();
			if(xawal != P1.getX()){
				std::swap(yawal, yakhir);
			}

			xawal += t1;
			xakhir += t1;
			yawal += t2;
			yakhir += t2;

			int selisihAbsX = xawal - xakhir >= 0 ? xawal - xakhir : xakhir - xawal;
			int selisihAbsY = yawal - yakhir >= 0 ? yawal - yakhir : yakhir - yawal;

			//bagi kasus kalo selisihnya 0
			if(selisihAbsX == 0){
				if(yawal > yakhir) std::swap(yawal, yakhir);
				for(int i = yawal; i <= yakhir; ++i){
					Point P = Point(xawal, i);
					gambarPoint(P, color);
				}
			}
			else if (selisihAbsY == 0){
				if(xawal > xakhir) std::swap(xawal, xakhir);
				for(int i = xawal; i <= xakhir; ++i){
					Point P = Point(i, yawal);
					gambarPoint(P, color);
				}
			}
			else if(selisihAbsY < selisihAbsX){
				if(xawal > xakhir) std::swap(xawal, xakhir), std::swap(yawal, yakhir);
				int deltaY = yakhir - yawal;
				int deltaX = xakhir - xawal;
				for(int i = 0; i <= deltaX; ++i){
					int modY = (deltaY * i) % deltaX;
					int ynow = yawal + (deltaY * i) / deltaX + (modY * 2) / deltaX;

					Point P = Point(xawal + i, ynow);
					gambarPoint(P, color);
				}
			}
			else{
				if(yawal > yakhir) std::swap(xawal, xakhir), std::swap(yawal, yakhir);
				int deltaY = yakhir - yawal;
				int deltaX = xakhir - xawal;
				for(int i = 0; i <= deltaY; ++i){
					int modX = (deltaX * i) % deltaY;
					int xnow = xawal + (deltaX * i) / deltaY + (modX * 2) / deltaY;

					Point P = Point(xnow, yawal + i);
					gambarPoint(P, color);
				}
			}

		}
	}
}

void Drawer::gambarPoint(const Point& P, Color color){
	int x = P.getX();
	int xd = x + xOffset;
	int y = P.getY();
	int yd = y + yOffset;
	bool performDraw = x >= 0 && y >= 0 && x < xClipWidth && y < yClipHeight;
	if (performDraw) {
		destination->setPixel(xd, yd, color);
	}

}
