/***************************************************************************
 *   Copyright (C) 2012 by Jérémie Dubois-Lacoste   *
 *   jeremie.dl@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "pfspinstance.h"
#include "Timer.hpp"

using namespace std;





int generateRndPosition(int min, int max)
{
	srand(1);
	return ( rand() % max + min );
}


/* Fill the solution with numbers between 1 and nbJobs, shuffled */
void randomPermutation(int nbJobs, vector< int > & sol)
{
	vector<bool> alreadyTaken(nbJobs+1, false); // nbJobs elements with value false
	vector<int > choosenNumber(nbJobs+1, 0);

	int nbj;
	int rnd, i, j, nbFalse;

	nbj = 0;
	for (i = nbJobs; i >= 1; --i)
	{
		rnd = generateRndPosition(1, i);
		nbFalse = 0;

		/* find the rndth cell with value = false : */
		for (j = 1; nbFalse < rnd; ++j)
			if ( ! alreadyTaken[j] )
				++nbFalse;
		--j;

		sol[j] = i;

		++nbj;
		choosenNumber[nbj] = j;

		alreadyTaken[j] = true;
	}
}

void simpleRZ(PfspInstance &instance, vector< int > & sol){

	std::vector< std::vector <long int> > matrix = instance.getMatrix();
	std::vector< long int > priority = instance.getPriority();
	std::vector< float> ti;
	std::vector< int> index;
	int nbJob = instance.getNbJob();
	int nbMac = instance.getNbMac();

	ti.resize(nbJob+1);
	ti[0]=0;
	index.resize(nbJob+1);
	index[0]=0;

	for (int j = 1; j <= nbJob; ++j)
		{
			float sum = 0;
			for (int m = 1; m <= nbMac; ++m)
			{
				sum+=matrix[j][m];
			}
			ti[j]=sum/priority[j];
			index[j]=j;
		}

		int i =1;
		sol[0]=0;
		while (ti.size() != 1){
			std::vector<float>::iterator result = std::min_element(ti.begin()+1, ti.end());
			int temp=std::distance(ti.begin(), result);
			sol[i]=index[temp];
			i++;
			ti.erase(ti.begin()+temp);
			index.erase(index.begin()+temp);
		}

		std::vector< int> jobs;
		jobs.resize(nbJob+1);
		jobs[1]=sol[1];
		int size=2;

		for (int i=2; i<=nbJob;i++){
			int temp = sol[i];
			int wct=0;
			size++;
			for (int j=1; j<size;j++){
				jobs.insert(jobs.begin()+j, temp);
				jobs.erase(jobs.end()-1);
				if (j==1){
					wct = instance.computeWCT(jobs);
					index = jobs;
				}
				if (wct > instance.computeWCT(jobs)){
					index = jobs;//we could already get the wct rather than recalculating afterwards
					wct = instance.computeWCT(jobs);
				}
				jobs.erase(jobs.begin()+j);
				jobs.insert(jobs.end(), 0);
			}
			jobs = index;
		}
		sol = index;

}

//TODO: NOT FINISHED
void Transpose(PfspInstance &instance, vector< int > & sol, bool first){

	int current=instance.computeWCT(sol);
	int best = current;
	//cout << "initial : " << best << endl;
	int precedent=best+1;//just for first iteration
	vector< int > copy;
	vector< int > best_sol=sol;

	while (best<precedent){
		precedent=best;
		//cout << "start :" << precedent << endl;
		sol = best_sol;
		for (int i=1;i<sol.size()-1;i++){
			copy = sol;
			//cout << i << endl;

			iter_swap(copy.begin() + i, copy.begin() + i+1);
			//std::swap(sol[1], sol[4]); // or swap with *(v.end()-1)

			current = instance.computeWCT(copy);
			//cout << current << endl;

			if (current < best){
				best_sol = copy;
				best = current;
				if (first){
					break;
				}
			}
		}
	}
	//cout << "afterwards :" << best << endl;
}

//TODO: if i==j pass
void Insert(PfspInstance instance, vector< int > & sol, bool first){
	
	int current=instance.computeWCT(sol);
	int best = current;
	//cout << "initial : " << best << endl;
	int precedent=best+1;//just for first iteration
	vector< int > copy;
	vector< int > best_sol = sol;//for first

	while (best<precedent){
		precedent=best;
		sol = best_sol;

		for (int j=1; j<sol.size();j++){
			for (int i=1;i<sol.size();i++){
				copy = sol;
				//cout << j << endl;
				//cout << i << endl;
				if (i!=j){
					// for (int k =1; k< sol.size();k++){
					// 	cout << copy[k] << " ";
					// }
					// cout << endl;
					if (i<j){
	        	std::rotate(copy.rend() - j - 1, copy.rend() - j, copy.rend() - i);
					}
					else{
						std::rotate(copy.begin() + j, copy.begin() + j+1, copy.begin()+i+1);//+i+1 for comprehension, since start is at 1 by convention
					}

					// for (int k =1; k< sol.size();k++){
					// 	cout << copy[k] << " ";
					// }
					// cout << endl;

					current = instance.computeWCT(copy);
					//cout << current << endl;

					if (current < best){
						best_sol = copy;
						best = current;
						if (first){
							sol = best_sol;
							break;
						}
					}
				}
			}
		}
	}
	//cout << "afterwards :" << best << endl;
}

//TODO: if i==j pass
void Exchange(PfspInstance instance, vector< int > & sol, bool first){
	
	int current=instance.computeWCT(sol);
	int best = current;
	//cout << "initial : " << best << endl;
	int precedent=best+1;//just for first iteration
	vector< int > copy;
	vector< int > best_sol = sol;//for first

	while (best<precedent){
		precedent=best;
		sol = best_sol;

		for (int j=1; j<sol.size();j++){
			for (int i=1;i<sol.size();i++){
				copy = sol;
				//cout << j << endl;
				//cout << i << endl;
				if (i!=j){
					// for (int k =1; k< sol.size();k++){
					// 	cout << copy[k] << " ";
					// }
					// cout << endl;
					
					iter_swap(copy.begin() + j, copy.begin()+i);

					// for (int k =1; k< sol.size();k++){
					// 	cout << copy[k] << " ";
					// }
					// cout << endl;

					current = instance.computeWCT(copy);
					//cout << current << endl;

					if (current < best){
						best_sol = copy;
						best = current;
						if (first){
							sol = best_sol;
							break;
						}
					}
				}
			}
		}
	}
	//cout << "afterwards :" << best << endl;
}


vector< int > Transp(PfspInstance instance, vector< int > & sol){
	int current=instance.computeWCT(sol);
	int best = current;
	//cout << "initial : " << best << endl;
	vector< int > copy;
	vector< int > best_sol = sol;//for first
	//Transpose
	for (int i=1;i<sol.size()-1;i++){
			copy = sol;
			//cout << i << endl;

			iter_swap(copy.begin() + i, copy.begin() + i+1);
			//std::swap(sol[1], sol[4]); // or swap with *(v.end()-1)

			current = instance.computeWCT(copy);
			//cout << current << endl;

			if (current < best){
				best_sol = copy;
				best = current;
				break;
			}
		}
	return best_sol;
}

vector< int > Exch(PfspInstance instance, vector< int > & sol){
	int current=instance.computeWCT(sol);
	int best = current;
	//cout << "initial : " << best << endl;
	vector< int > copy;
	vector< int > best_sol = sol;//for first
	//Transpose
	for (int j=1; j<sol.size();j++){
			for (int i=1;i<sol.size();i++){
				copy = sol;
				if (i!=j){
					iter_swap(copy.begin() + j, copy.begin()+i);
					current = instance.computeWCT(copy);
					//cout << current << endl;

					if (current < best){
						best_sol = copy;
						best = current;
						break;
					}
				}
			}
		}
	return best_sol;
}

vector< int > Ins(PfspInstance instance, vector< int > & sol){
	int current=instance.computeWCT(sol);
	int best = current;
	//cout << "initial : " << best << endl;
	vector< int > copy;
	vector< int > best_sol = sol;//for first
	//Transpose
	for (int j=1; j<sol.size();j++){
			for (int i=1;i<sol.size();i++){
				copy = sol;
				if (i!=j){
					if (i<j){
	        	std::rotate(copy.rend() - j - 1, copy.rend() - j, copy.rend() - i);
					}
					else{
						std::rotate(copy.begin() + j, copy.begin() + j+1, copy.begin()+i+1);//+i+1 for comprehension, since start is at 1 by convention
					}

					current = instance.computeWCT(copy);
					if (current < best){
						best_sol = copy;
						best = current;
						break;
						
					}
				}
			}
		}
	return best_sol;
}

void VND(PfspInstance instance, vector< int > & sol){

	vector< int > copy;
	int best = instance.computeWCT(sol);

	start:
	copy = Transp(instance, sol);
	int current = instance.computeWCT(copy);
	if (current < best){
		//cout << "transpose" << endl;
		sol=copy;
		best=current;
		goto start;
	}
	copy = Exch(instance, sol);
	current = instance.computeWCT(copy);
	if (current < best){
		//cout << "insert" << endl;
		sol=copy;
		best=current;
		goto start;
	}
	copy = Ins(instance, sol);
	current = instance.computeWCT(copy);
	if (current < best){
		//cout << "exch" << endl;
		sol=copy;
		best=current;
		goto start;
	}
	//cout << best << end;
}

/***********************************************************************/

int main(int argc, char *argv[])
{
	int i;
	long int WeightedSumCompletionTimes;
	bool first;

	//cout << "\n\n" << endl;


	if (argc <= 4)
	{
		cout << "Usage: ./flowshopWCT <instance_file> <first/best> <transpose/exchange/insert> <random/srz>" << endl;
		return 0;
	}

	string sec=argv[2];
	string third = argv[3];
	string fourth = argv[4];

	if (sec=="first"){
		//cout << "first"<< endl;
		first = true;
	}
	else if (sec=="best"){
		//cout << "best"<< endl;
		first=false;
	}
	else{
		cout << "Usage: ./flowshopWCT <instance_file> <first/best> <transpose/exchange/insert> <random/srz>" << endl;
		return 0;
	}

	/* initialize random seed: */
	srand ( time(NULL) );

	/* Create instance object */
	PfspInstance instance;

	/* Read data from file */
	if (! instance.readDataFromFile(argv[1]) )
		return 1;
	argv[1]+=13;
	cout << argv[1] << " , ";
	/* Create a vector of int to represent the solution
		WARNING: By convention, we store the jobs starting from index 1,
						 thus the size nbJob + 1. */
	vector< int > solution ( instance.getNbJob()+ 1 );

	/* Fill the vector with a random permutation */
	float average=0;
	Timer timer;

	if (fourth=="random"){
		randomPermutation(instance.getNbJob(), solution);
		// cout << "Random solution: " ;
		// for (i = 1; i <= instance.getNbJob(); ++i)
		// 	cout << solution[i] << " " ;
		// cout << endl;
	}
	else if (fourth=="srz"){
		simpleRZ(instance, solution);
		// cout << "Simplified RZ solution: " ;
		// for (i = 1; i <= instance.getNbJob(); ++i)
		// 	cout << solution[i] << " " ;
		// cout << endl;
	}
	else{
		cout << "Usage: ./flowshopWCT <instance_file> <first/best> <transpose/exchange/insert> <random/srz>" << endl;
		return 0;
	}

	VND(instance, solution);


	if (third=="transpose"){
		//cout << "transpose"<< endl;
		//Transpose(instance, solution, first);
		//cout << endl;
		//cout << endl;
	}
	else if (third=="insert"){
		//cout << "insert"<< endl;
		//Insert(instance, solution, first);
		//cout << endl;
		//cout << endl;
	}
	else if (third=="exchange"){
		//cout << "exchange"<< endl;
		//Exchange(instance, solution, first);
		//cout << endl;
		//cout << endl;
	}
	else{
		cout << "Usage: ./flowshopWCT <instance_file> <first/best> <transpose/exchange/insert> <random/srz>" << endl;
		return 0;
	}
	timer.Stop();
	average+=timer.getDuration();
	//cout << "average : " << average << " ms" << endl;

	/* Compute the WCT of this solution */

	WeightedSumCompletionTimes = instance.computeWCT(solution);
	//cout << "Total weighted completion time: " << WeightedSumCompletionTimes << endl;
	cout << WeightedSumCompletionTimes << endl;
	return 0;
}
