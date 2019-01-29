#pragma once


using namespace System;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Collections;
using namespace System::Collections::Generic;

// Return the bounds of the list of points.
System::Drawing::Rectangle GetPointListBounds(List<System::Drawing::Point>^points)
{
	int xmin = points[0].X;
	int xmax = xmin;
	int ymin = points[0].Y;
	int ymax = ymin;

	for (int i = 1; i < points->Count; i++)
	{
		if (xmin > points[i].X) xmin = points[i].X;
		if (xmax < points[i].X) xmax = points[i].X;
		if (ymin > points[i].Y) ymin = points[i].Y;
		if (ymax < points[i].Y) ymax = points[i].Y;
	}

	return System::Drawing::Rectangle(xmin, ymin, xmax - xmin, ymax - ymin);
}

// Copy the selected piece of the image into a new bitmap.
Bitmap^ GetSelectedArea(Image^ source,Color bg_color,List<System::Drawing::Point>^points)
{
	// Make a new bitmap that has the background
	// color except in the selected area.
	Bitmap^ big_bm = gcnew Bitmap(source);
	Graphics^gr = Graphics::FromImage(big_bm);
	{
		// Set the background color.
		gr->Clear(bg_color);

		// Make a brush out of the original image.
		Brush^ br = gcnew TextureBrush(source);
		// Fill the selected area with the brush.
		int l_iNumPolygon = points->Count/3;
		for (int i = 0; i < l_iNumPolygon; ++i)
		{
			List<System::Drawing::Point>^l_pTrianglePoints = gcnew List<System::Drawing::Point>();
			for (int j = 0; j < 3; ++j)
			{
				int l_iIndex = i * 3 + j;
				l_pTrianglePoints->Add(points[l_iIndex]);
			}
			gr->FillPolygon(br, l_pTrianglePoints->ToArray());
		}

		// Find the bounds of the selected area.
		System::Drawing::Rectangle source_rect = GetPointListBounds(points);

		// Make a bitmap that only holds the selected area.
		//Bitmap^result = gcnew Bitmap(source_rect.Width, source_rect.Height);
		Bitmap^result = gcnew Bitmap(source->Width, source->Height);
		// Copy the selected area to the result bitmap.
		Graphics^result_gr = Graphics::FromImage(result);

		System::Drawing::Rectangle dest_rect = System::Drawing::Rectangle(source_rect.X, source_rect.Y,source_rect.Width, source_rect.Height);
		result_gr->DrawImage(big_bm, dest_rect,source_rect, GraphicsUnit::Pixel);

		// Return the result.
		return result;
	}
}

List<System::Drawing::Point>^	Vector2ToListPoint(std::vector<Vector2>*e_pVector)
{
	List<System::Drawing::Point>^l_pList = gcnew List<System::Drawing::Point>();
	for (size_t i = 0; i<e_pVector->size(); ++i)
	{
		auto l_pData = (*e_pVector)[i];
		l_pList->Add(System::Drawing::Point((int)l_pData.x, (int)l_pData.y));
	}
	return l_pList;
}