#include<opencv2/opencv.hpp>
#include <iostream>
#include "Timer.h"
#include <queue>
#include <vector>
#include <climits>
#include <cstring>

using namespace std;

//cv::Mat image = cv::imread("10x10.png", cv::IMREAD_GRAYSCALE);
cv::Mat image = cv::imread("labyrinth.png", cv::IMREAD_GRAYSCALE);
#define M image.rows
#define N image.cols

int row[] = { -1, 0, 0, 1 };
int col[] = { 0, -1, 1, 0 };

class Node {
public:
	Node(int i, int j, int dist, Node* parent) :
		_i(i), _j(j), _dist(dist), _parent(parent) {};
public:
	int _i, _j, _dist;
	Node *_parent;
};

bool isValid(const cv::Mat& image, vector<vector<char>>& visited, int row, int col)
{
	return (row >= 0) && (row < M) && (col >= 0) && (col < N)
		&& image.at<uint8_t>(row, col) && visited[row][col]!='1';
}

void BFS(cv::Mat& image, int i, int j, int x, int y)
{
	vector<vector<char>> visited(M, vector<char>(N));
	queue<Node*> q;

	visited[i][j] = '1';
	q.push(&Node(i, j, 0, nullptr));
	int min_dist = INT_MAX;

	while (!q.empty())
	{
		Node *node = q.front();
		q.pop();
		int i = node->_i, j = node->_j, dist = node->_dist;

		if (i == x && j == y)
		{
			cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
			min_dist = dist;
			while (node) {
				image.at<cv::Vec3b>(node->_i, node->_j)[0] = 0;
				image.at<cv::Vec3b>(node->_i, node->_j)[1] = 0;
				image.at<cv::Vec3b>(node->_i, node->_j)[2] = 255;
				node = node->_parent;
			}
			break;
		}
		for (int k = 0; k < 4; k++)
		{
			if (isValid(image, visited, i + row[k], j + col[k]))
			{
				Node *n=new Node(i + row[k], j + col[k], dist + 1, node);
				q.push(n);
				visited[i + row[k]][j + col[k]] = '1';
			}
		}
	}

	if (min_dist != INT_MAX)
		cout << "The shortest path from source to destination "
		"has length " << min_dist << endl;
	else
		cout << "Destination can't be reached from given source" << endl;
}

int main()
{
	{
		Timer t;
		for (int i = 0; i < image.rows; i++)
			for (int j = 0; j < image.cols; j++)
				image.at<uint8_t>(i, j) = (image.at<uint8_t>(i, j) < 200 ? 0 : 255);
		BFS(image, 4, 0, image.rows - 3, image.cols - 1);
	}

	cv::namedWindow("Original Image", cv::WINDOW_NORMAL);
	imshow("Original Image", image);


	cv::waitKey();
	return 0;
}

