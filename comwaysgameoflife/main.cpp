#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>

// declaring the consts

const int WIDTH = 1920;
const int HEIGHT = 1080;

const int GRIDSIZE = 10;

const int NUM_CELLS_X = WIDTH / GRIDSIZE;
const int NUM_CELLS_Y = HEIGHT / GRIDSIZE;

int countAlive(const std::vector<std::vector<bool>>& grid, const int row, const int col)
{
	int count = 0;
	int rows = grid.size();
	int cols = grid[0].size();

	// Check all eight neighboring cells
	for (int dr = -1; dr <= 1; ++dr) {
		for (int dc = -1; dc <= 1; ++dc) {
			if (dr == 0 && dc == 0) continue;  // Skip the current cell
			int r = row + dr;
			int c = col + dc;

			// Ensure the row and column are within bounds
			if (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c]) {
				count++;
			}
		}
	}

	return count;
}


void fate(const std::vector<std::vector<bool>>& grid, std::vector<std::pair<int, int>>& toBorn, std::vector<std::pair<int, int>>& toDie)
{
	toBorn.clear();
	toDie.clear();
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			int n = countAlive(grid, i, j);

			if (n < 2 || n > 3)
				toDie.push_back(std::make_pair(i, j));
			else if (n == 3)
				toBorn.push_back(std::make_pair(i, j));
		}
	}
}

void update(std::vector<std::vector<bool>>& grid, const std::vector<std::pair<int, int>>& toBorn, const std::vector<std::pair<int, int>>& toDie)
{
	for (const std::pair<int, int>& idx : toBorn)
	{
		grid[idx.first][idx.second] = true;
	}
	for (const std::pair<int, int>& idx : toDie)
	{
		grid[idx.first][idx.second] = false;
	}
}

void drawWindow(sf::RenderWindow& window, std::vector<std::vector<bool>>& grid)
{
	sf::Font font;
	font.loadFromFile("font.ttf");

	sf::Text text;
	text.setFont(font);
	text.setString("Press enter when u are done drawing");
	text.setCharacterSize(20);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Enter) return;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i pos = sf::Mouse::getPosition(window);

			int x = pos.x / GRIDSIZE;
			int y = pos.y / GRIDSIZE;

			if (x >= 0 && x < NUM_CELLS_X && y >= 0 && y < NUM_CELLS_Y)
				grid[x][y] = true;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			sf::Vector2i pos = sf::Mouse::getPosition(window);

			int x = pos.x / GRIDSIZE;
			int y = pos.y / GRIDSIZE;

			if (x >= 0 && x < NUM_CELLS_X && y >= 0 && y < NUM_CELLS_Y)
				grid[x][y] = false;
		}

		window.clear();

		window.draw(text);

		for (int i = 0; i < grid.size(); i++)
		{
			for (int j = 0; j < grid[i].size(); j++)
			{
				if (grid[i][j])
				{
					sf::RectangleShape block(sf::Vector2f(GRIDSIZE - 1, GRIDSIZE - 1));
					block.setPosition(i * GRIDSIZE, j * GRIDSIZE);
					window.draw(block);
				}
			}
		}

		window.display();
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Comway's Game of Life", sf::Style::Fullscreen);


std::vector<std::vector<bool>> grid(NUM_CELLS_X, std::vector<bool>(NUM_CELLS_Y));
	std::vector<std::pair<int, int>> toBorn;
	std::vector<std::pair<int, int>> toDie;

	drawWindow(window, grid);

	window.setFramerateLimit(20);

	sf::RectangleShape block(sf::Vector2f(GRIDSIZE - 1, GRIDSIZE - 1));

	sf::Font font;
	
	font.loadFromFile("font.ttf");

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color(255,255,255,128));

	int generation = 0;
	

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
		}

		generation++;

		fate(grid, toBorn, toDie);
		update(grid, toBorn, toDie);

		text.setString("Generation: " + std::to_string(generation));

		window.clear();
		
		for (int i = 0; i < grid.size(); i++)
		{
			for (int j = 0; j < grid[i].size(); j++)
			{
				if (grid[i][j])
				{
					block.setPosition(i * GRIDSIZE, j * GRIDSIZE);
					window.draw(block);
				}
			}
		}

		window.draw(text);

		window.display();
	}

}
