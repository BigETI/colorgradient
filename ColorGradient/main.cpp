#include <iostream>
#include <stdint.h>
#include <string>
#include "resource.h"

#define TGA_CMP_HEADER_LEN	(12)

static char tga_header[TGA_CMP_HEADER_LEN] = { 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

int main()
{
	unsigned short i, j;
	wchar_t buf[512];
	std::wstring output_file_name;
	unsigned __int32 out_file_id = 0, progress = 0;
	struct Pixel
	{
		unsigned char blue, green, red;
	} pixel_data;
	unsigned char color_buf;
	FILE *w_file = NULL;
	struct IHDR_chunk
	{
		unsigned short width, height;
		unsigned char bpp, dummy;
	} ihdr_chunk_buffer;
	ihdr_chunk_buffer.width = 0;
	ihdr_chunk_buffer.height = 0;
	ihdr_chunk_buffer.bpp = sizeof(Pixel) * 8;
	ihdr_chunk_buffer.dummy = 0;
	while (!ihdr_chunk_buffer.width)
	{
		std::wcout << L"Please choose a width for the color gradient to generate: ";
		memset(buf, 0, sizeof(wchar_t) * 512);
		std::wcin >> buf;
		ihdr_chunk_buffer.width = _wtoi(buf);
		std::wcout << L"\n\n";
	}
	while (!ihdr_chunk_buffer.height)
	{
		std::wcout << L"Now choose a height for the color gradient to generate: ";
		memset(buf, 0, sizeof(wchar_t) * 512);
		std::wcin >> buf;
		ihdr_chunk_buffer.height = _wtoi(buf);
		std::wcout << L"\n\n";
	}
	std::wcout << L"Generating ";
	while (true)
	{
		output_file_name = L"color_gradient_";
		output_file_name += std::to_wstring(out_file_id);
		output_file_name += L".tga";
		_wfopen_s(&w_file, output_file_name.c_str(), L"rb");
		if (w_file)
		{
			fclose(w_file);
			out_file_id++;
		}
		else
		{
			_wfopen_s(&w_file, output_file_name.c_str(), L"wb");
			break;
		}
	}
	std::wcout << L'\"' << output_file_name << L"\"...";
	fwrite(tga_header, sizeof(char), TGA_CMP_HEADER_LEN, w_file);
	fwrite(&ihdr_chunk_buffer, sizeof(IHDR_chunk), 1, w_file);
	for (i = 0; i != ihdr_chunk_buffer.height; i++)
	{
		for (j = 0; j != ihdr_chunk_buffer.width; j++)
		{
			// Human Eye
			pixel_data.green = ((unsigned char)((~((j * 0xFF) / ihdr_chunk_buffer.width)) & 0xFF));
			color_buf = ((unsigned char)(((i * 0xFF) / ihdr_chunk_buffer.height) & 0xFF));
			pixel_data.green = (pixel_data.green <= color_buf) ? 0x0 : (pixel_data.green - color_buf);

			pixel_data.red = ((unsigned char)((~((j * 0xFF) / ihdr_chunk_buffer.width)) & 0xFF));
			color_buf = ((unsigned char)((~((i * 0xFF) / ihdr_chunk_buffer.height)) & 0xFF));
			pixel_data.red = (pixel_data.red <= color_buf) ? 0x0 : (pixel_data.red - color_buf);

			pixel_data.blue = ((unsigned char)(((j * 0xFF) / ihdr_chunk_buffer.width) & 0xFF));
			color_buf = ((unsigned char)(((i * 0xFF) / ihdr_chunk_buffer.height) & 0xFF));
			pixel_data.blue = (pixel_data.blue <= color_buf) ? 0x0 : (pixel_data.blue - color_buf);

			/*if (j > (ihdr_chunk_buffer.width / 2)) pixel_data.red = 0x0;
			else pixel_data.red = ((unsigned char)((~((j * 0xFF) / (ihdr_chunk_buffer.width / 2))) & 0xFF));

			pixel_data.green = 0;

			if (((ihdr_chunk_buffer.width - j)) > (ihdr_chunk_buffer.width / 2)) pixel_data.blue = 0x0;
			else pixel_data.blue = ((unsigned char)(((j * 0xFF) / (ihdr_chunk_buffer.width / 2)) & 0xFF));*/

			fwrite(&pixel_data, sizeof(Pixel), 1, w_file);
		}
	}
	fclose(w_file);
	return 0;
}