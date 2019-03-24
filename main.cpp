#include "RBTree.h"
#include <ctime>

using namespace std;

int main()
{
	RBTree<int> tree;
	tree.insert(10);
	tree.insert(20);
	tree.insert(-10);
	tree.insert(15);
	int count = tree.insert(17); 
	tree.insert(40);
	tree.insert(50);
	tree.insert(60);
	tree.insert(38);
	tree.insert(5);
	tree.insert(-20);
	tree.insert(7);
	tree.insert(4);

	for (auto x = tree.begin(); x != tree.end(); ++x)
		cout << (*x)->data << "(" << (*x)->nodeColor << ") ";
	cout << endl;
	tree.print_stack();
	
	cout << endl << "Count rotations for insertion 17: " << count << endl;

	count = tree.deleteNode(-10);
	cout << "Delete element -10" << endl;
	tree.print_stack();
	cout << endl;
	for (auto x = tree.begin(); x != tree.end(); ++x)
		cout << (*x)->data << "(" << (*x)->nodeColor << ") ";

	cout << endl << "Count rotations for deletion -10: " << count << endl;

	cout << "Find element 5: " << (*tree.find(5))->data << endl;

	/*cout << "\n\n" << "========= Red-black trees are tested ==========" << "\n\n";

	RBTree<int> tree2;
	set<int> s;
	vector<int> rand_vector;
	for (int i = 0; i < 30000; i++) {
		int x = rand() % 10000000 + 1;
		rand_vector.push_back(x);
		tree2.insert(x);
		s.insert(x);
	}

	cout << tree2.tree_height() << " ~ " << log(30000) << "\n";

	cout << "Time to find elements not containing in RBTree: ";
	int t1 = clock();
	for (int i = 10000001; i < 100010001; i++) {
		auto it = tree2.find(i);
	}
	int t2 = clock();
	cout << double((t2 - t1) * 1000 / CLOCKS_PER_SEC) << " ms" << endl;

	cout << "Time to find elements not containing in set: ";
	t1 = clock();
	for (int i = 10000001; i < 100010001; i++) {
		auto it = s.find(i);
	}
	t2 = clock();
	cout << double((t2 - t1) * 1000 / CLOCKS_PER_SEC) << " ms" << endl;

	cout << "Time to find existing elements in RBTree: ";
	t1 = clock();
	for (auto x : rand_vector) {
		auto it = tree2.find(x);
	}
	t2 = clock();
	cout << double((t2 - t1) * 1000 / CLOCKS_PER_SEC) << " ms" << endl;

	cout << "Time to find existing elements in set: ";
	t1 = clock();
	for (auto x : rand_vector) {
		auto it = s.find(x);
	}
	t2 = clock();
	cout << double((t2 - t1) * 1000 / CLOCKS_PER_SEC) << " ms" << endl;

	rand_vector.resize(30000);
	for (auto& x : rand_vector) x = 1000000 + rand() % 1000000000;
	cout << "Time to find random elements in RBTree: ";
	t1 = clock();
	for (auto x : rand_vector) {
		auto it = tree2.find(x);
	}
	t2 = clock();
	cout << double((t2 - t1) * 1000 / CLOCKS_PER_SEC) << " ms" << endl;

	cout << "Time to find random elements in set: ";
	t1 = clock();
	for (auto x : rand_vector) {
		auto it = s.find(x);
	}
	t2 = clock();
	cout << double((t2 - t1) * 1000 / CLOCKS_PER_SEC) << " ms" << endl;*/

	system("pause");
}

/*
-20(0) -10(1) 4(1) 5(0) 7(1) 10(0) 15(0) 17(0) 20(0) 38(1) 40(0) 50(0) 60(1)
60 50 40 38 20 17 15 10 7 5 4 -10 -20
Count rotations for insertion 17: 2
Delete element -10
60 50 40 38 20 17 15 10 7 5 4 -20
-20(0) 4(1) 5(0) 7(1) 10(0) 15(0) 17(0) 20(0) 38(1) 40(0) 50(0) 60(1)
Count rotations for deletion -10: 0
Find element 5: 5
60 50 40 38 20 17 15 10 7 5 4 -20
 */