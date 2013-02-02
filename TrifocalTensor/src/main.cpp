#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <algorithm>

#include <cstring>
#include <Eigen/SVD>

#include "MathIO.hpp"
#include "draw.hpp"

#include "Tensor.hpp"
#include "Transfert.hpp"
#include "zoom.hpp"

#define NB_IMG 3

using namespace std;

int main(int argc, char *argv[]){

	/************************************
	 *----- DISPLAY OPTIONS PASSED -----*
	 ************************************/
	int image_count = 0;
	int list_count = 0;
	int j;
	for (j=0; j < argc; ++j){
		/* If help needed */
		if(strcmp( argv[j], "-h") == 0 || strcmp( argv[j], "-help") == 0 ){
			//Executing a command line via a bash script
            char script[20] = "./script.sh";
            char ** ptrArgs = (char**)malloc(2*sizeof(char*));
            ptrArgs[0] = script;
            ptrArgs[1] = NULL;
			execv(script, ptrArgs);

		}else if(strcmp( argv[j], "-h") != 0 && strcmp( argv[j], "-help") != 0 ){

			/* If no arguments passed */
			if(argc < 2){
				cout << "-----------------------------------------------------------------------------" << endl;
				cout << "		TRIFOCAL TENSOR  -  WELCOME ! 																						" 
									<< " 		Please specify the names of image files or check the help (-h)	" << endl;
				cout	<< "-----------------------------------------------------------------------------" << endl;
				return 0;
			}

			/* Check image files */			
			for(int i=1; i < 4 ; ++i){
				if(argv[i]!=NULL){
					string file = argv[i];
					if(file.substr(file.find_last_of(".") + 1) == "jpg" || file.substr(file.find_last_of(".") + 1) == "png") {
							image_count ++;
					} 
					else{
						cout << "-----------------------------------------------------------------------------" << endl;
						cout << "		TRIFOCAL TENSOR 	-  OH NO...													" 
											<< " 		You specified a non valid format of image files.						"
											<< " 		Please try again or check the help (-h)											" << endl;
						cout	<< "-----------------------------------------------------------------------------" << endl;
					}
				}
				else{
					//Check if too few or too many image files passed
					cout << "-----------------------------------------------------------------------------" << endl;
					cout << "		TRIFOCAL TENSOR 	-  TRY AGAIN !!											" 
										<< " 		You specified too few or too many image files : 3 needed.						"
										<< " 		Please try again or check the help (-h)											" << endl;
					cout	<< "-----------------------------------------------------------------------------" << endl;
				}
			}

			/* Check list files */			
			for(int i=4; i < argc ; ++i){
				string file = argv[i];
				if(file.substr(file.find_last_of(".") + 1) == "list"){
						list_count ++;
				}
			}
			if(list_count > 3){//Check if too many list files passed
				cout << "-----------------------------------------------------------------------------" << endl;
				cout << "		TRIFOCAL TENSOR 	-  OH NO...													" 
									<< " 		You specified too many list files : 3 maximum.						"
									<< " 		Please try again or check the help (-h)											" << endl;
				cout	<< "-----------------------------------------------------------------------------" << endl;
			}

			cout << "Image Files : "<< image_count << " List Files : " << list_count  << endl;
		}


		/*******************************
		 *----- START APPLICATION -----*
		 *******************************/

		if(image_count == 3){

			/* Init SDL image */
			if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == -1){
				cerr << "error IMG_Init" << endl;
				return EXIT_FAILURE;
			}

			/* Load some images */
			SDL_Surface *image1 = IMG_Load(argv[1]);
			SDL_Surface *image2 = IMG_Load(argv[2]);
			SDL_Surface *image3 = IMG_Load(argv[3]);
			if(image1 == 0 || image2 == 0 || image3 == 0){
			  cerr << "error loading images" << endl;
			  return 0;
			}

			/* Init screen surface */
			if(SDL_Init(SDL_INIT_VIDEO) == -1){
				cerr << "error SDL_Init" << endl;
				return EXIT_FAILURE;
			}

			/* Create a screen surface that will include the 3 images */
			SDL_Surface *screen = SDL_SetVideoMode(image1->w + image2->w + image3->w, image1->h, 32, SDL_HWSURFACE);
			SDL_WM_SetCaption("Trifocal Tensor", NULL);

			/* bibd the images on the surface */
			SDL_Rect imageOffset;
			imageOffset.x = 0;
			imageOffset.y = 0;
			SDL_BlitSurface(image1, NULL, screen, &imageOffset);
			imageOffset.x = image1->w;
			SDL_BlitSurface(image2, NULL, screen, &imageOffset);
			imageOffset.x = image1->w + image2->w;
			SDL_BlitSurface(image3, NULL, screen, &imageOffset);

            /* Declare variables */

			Tensor T;

            /*Lists */
            int matching_points = 0;
			Eigen::MatrixXd list1;
			Eigen::MatrixXd list2;
			Eigen::MatrixXd list3;

            int nb_corresp;

			// Colors
			Uint32 red  = 0xffff3333;
            Uint32 green  = 0xff33ff33;
			Uint32 blue = 0xff3333ff;
			Uint32 yellow = 0xffffff33;

            /* Load the point lists */
            if(list_count > 0){


                kn::loadMatrix(list1,argv[4]);
                kn::loadMatrix(list2,argv[5]);
                kn::loadMatrix(list3,argv[6]);

                // draw points on image1
                for(int i=0; i<list1.rows(); ++i)
                    fill_circle(screen, list1(i,0), list1(i,1), 3, red);

                // draw points on image2
                for(int i=0; i<list2.rows(); ++i)
                    fill_circle(screen, list2(i,0)+image1->w, list2(i,1), 3, blue);

                // draw points on image3
                for(int i=0; i<list3.rows(); ++i)
                    fill_circle(screen, list3(i,0)+image1->w+image2->w, list3(i,1), 3, yellow);
                
                nb_corresp = min(min(list1.rows(), list2.rows()), list3.rows());
                matching_points = 1;

				/*****************************
				 *----- TRIFOCAL TENSOR -----*
				 *****************************/

				Eigen::Vector3d point1;
				Eigen::Vector3d point2;
				Eigen::Vector3d point3;

				/* Matrix A */
				Eigen::MatrixXd A = MatrixXd::Zero(4*nb_corresp, 27);

				/* Fill matrix A */
                
                for(int i=0; i<=1; ++i){
                    for(int l=0; l<=1 ; ++l){
                        for(int p=0; p<nb_corresp; ++p){
                            for(int k=0; k<=2; ++k){

                            point1 << list1(p,0), list1(p,1), list1(p,2);
                            point2 << list2(p,0), list2(p,1), list2(p,2);
                            point3 << list3(p,0), list3(p,1), list3(p,2);

                            A(4*p + 2*i + l, 9*k + 3*i + l) = -point1(k)*point2(2)*point3(2);
                            A(4*p + 2*i + l, 9*k + 3*i + 2) = point1(k)*point2(2)*point3(l);
                            A(4*p + 2*i + l, 9*k + 6 + l) = point1(k)*point2(i)*point3(2);
                            A(4*p + 2*i + l, 9*k + 8) = -point1(k)*point2(i)*point3(l);

                            // EQ += point1(k)*(point2(i)*point3(2)*T(2,l,k) - point2(2)*point3(2)*T(i,l,k) - point2(i)*point3(l)*T(2,2,k) + point2(2)*point3(l)*T(i,2,k));

                            }
                        }
                    }
                }


				/*----- SVD -----*/

				Eigen::JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
				//cout << "Singular values : " << endl << svd.singularValues() << endl;
				//cout << "U Matrix : " << endl << svd.matrixU() << endl;
				Eigen::MatrixXd V = svd.matrixV();
                Eigen::VectorXd t = V.col(26);

                T.fillWith(t);
				//T.print();

            }

            if(!matching_points){
                ofstream file_list1("input/list1.list", ios_base::trunc);
                file_list1.close();
                ofstream file_list2("input/list2.list", ios_base::trunc);
                file_list2.close();
                ofstream file_list3("input/list3.list", ios_base::trunc);
                file_list3.close();
                cout << "Please, select at least seven matching points" << endl;
                cout << "To exit point selection mode, press Enter" << endl;
            }

			/*********************
			 *----- DISPLAY -----*
			 *********************/

			SDL_Flip(screen);

			int loop = 1;
			int x, y;
			int right_x, right_y;
            int clicks = 0;
            int img1=0, img2=0, img3=0;



            Eigen::Vector3d point1_clicked;
            Eigen::Vector3d point2_clicked;
            Eigen::Vector3d point3_clicked;
            Eigen::VectorXd new_point;

			while(loop){

				SDL_Event e;
				while(SDL_PollEvent(&e)) {

					if(e.type == SDL_MOUSEBUTTONDOWN){
                    	if(e.button.button == SDL_BUTTON_LEFT){					
                    		SDL_GetMouseState(&x, &y);

                    		/* CLICK ON IMAGE 2 */
                    		if (x > image1->w && x <= (image1->w + image2->w)) {

                                /* Save mouse coordinates */
                                point2_clicked << x-image1->w, y, 1;
                                /* If at least 7 matching points */
                                if(matching_points){
                                	if (clicks == 0){
                                		clicks++;
                                        img2 = 1;
                                	}
                                	else {
                                		clicks = 0;
                                        if(img1){
                                            new_point = transfertTo3(point1_clicked, point2_clicked, T);
                                    		SDL_Flip(screen);
                                    		fill_circle(screen, new_point(0)+image1->w+image2->w, new_point(1), 3, green);
                                    		SDL_Flip(screen);
                                        }
                                        if(img3){
                                            new_point = transfertTo1(point2_clicked, point3_clicked, T);
                                            SDL_Flip(screen);
                                            fill_circle(screen, new_point(0), new_point(1), 3, green);
                                            SDL_Flip(screen);
                                        }

                                        img1 = 0;
                                        img2 = 0;
                                        img3 = 0;
                                	}
                                }
                                else{
                                    ofstream file_list2;
                                    if(!file_list2) { 
                                        cout << "Cannot open file" << endl; 
                                        return 1; 
                                    }
                                    file_list2.open("input/list2.list", ios::app);
                                    file_list2 << x-image1->w << " " << y << " " << "1" << endl;
                                    file_list2.close();
                                }

                            	/* Display point */
                            	SDL_Flip(screen);
                            	fill_circle(screen, x, y, 3, blue);
                            	SDL_Flip(screen);
                    		}

                    		/* CLICK ON IMAGE 3 */
                    		else if(x > (image1->w + image2->w)) {

                                /* Save mouse coordinates */
                                point3_clicked << x-(image1->w+image2->w), y, 1;

                                /* If at least 7 matching points */
                                if(matching_points){
                        			if (clicks == 0){
                                		clicks++;
                                        img3 = 1;
                                	}
                                	else {
                                		clicks = 0;
                                        if(img2){
                                		    new_point = transfertTo1(point2_clicked, point3_clicked, T);
                                    		SDL_Flip(screen);
                                    		fill_circle(screen, new_point(0), new_point(1), 3, green);
                                    		SDL_Flip(screen);
                                        }
                                        if(img1){
                                            new_point = transfertTo2(point1_clicked, point3_clicked, T);
                                            SDL_Flip(screen);
                                            fill_circle(screen, new_point(0)+image1->w, new_point(1), 3, green);
                                            SDL_Flip(screen);
                                        }
                                        img1 = 0;
                                        img2 = 0;
                                        img3 = 0;
                                	}
                                }
                                else{
                                    ofstream file_list3;
                                    if(!file_list3) { 
                                        cout << "Cannot open file" << endl; 
                                        return 1; 
                                    }
                                    file_list3.open("input/list3.list", ios::app);
                                    file_list3 << x-(image1->w+image2->w) << " " << y << " " << "1" << endl;
                                    file_list3.close();
                                }

                            	/* Display point */
                    			SDL_Flip(screen);
                    			fill_circle(screen, x, y, 3, yellow);
                    			SDL_Flip(screen);
                    		}

                    		/* CLICK ON IMAGE 1 */
                    		else {

                                /* Save mouse coordinates */
                                point1_clicked << x, y, 1;

                                /* If at least 7 matching points */
                                if(matching_points){
                        			if (clicks == 0){
                                		clicks++;
                                        img1 = 1;
                                	}
                                	else {
                                        clicks = 0;

                                        if(img3){
                                            new_point = transfertTo2(point1_clicked, point3_clicked, T);
                                            SDL_Flip(screen);
                                            fill_circle(screen, new_point(0)+image1->w, new_point(1), 3, green);
                                            SDL_Flip(screen);
                                        }if(img2){
                                            new_point = transfertTo3(point1_clicked, point2_clicked, T); 
                                            SDL_Flip(screen);
                                            fill_circle(screen, new_point(0)+image1->w+image2->w, new_point(1), 3, green);
                                            SDL_Flip(screen);
                                        }

                                        img1 = 0;
                                        img2 = 0;
                                        img3 = 0;
                                	}
                                }
                                else{
                                    ofstream file_list1;
                                    if(!file_list1) { 
                                        cout << "Cannot open file" << endl; 
                                        return 1; 
                                    }
                                    file_list1.open("input/list1.list", ios::app);
                                    file_list1 << x << " " << y << " " << "1" << endl;
                                    file_list1.close();
                                }

                            	/* Display point */
                    			SDL_Flip(screen);
                    			fill_circle(screen, x, y, 3, red);
                    			SDL_Flip(screen);
                    		}
                        }
                        if(e.button.button == SDL_BUTTON_RIGHT){
                        	SDL_GetMouseState(&right_x, &right_y);
                        	//cout << "x " << right_x <<  " y " << right_y << endl;
                        	myZoom(0, image3, right_x, right_y, screen, &imageOffset);
                        	
                        }
                    }

                    if(e.type == SDL_KEYDOWN){
                        if(e.key.keysym.sym == SDLK_RETURN){

                            /* Count lines of list1 */
                            int numLines1 = 0;
                            char c;
                            ifstream file_list1("input/list1.list");
                            while (file_list1.good()){
                                c = file_list1.get();
                                if(c=='\n') numLines1++;
                            }

                            /* Count lines of list2 */
                            int numLines2 = 0;
                            ifstream file_list2("input/list2.list");
                            while (file_list2.good()){
                                c = file_list2.get();
                                if(c=='\n') numLines2++;
                            }

                            /* Count lines of list3 */
                            int numLines3 = 0;
                            ifstream file_list3("input/list3.list");
                            while (file_list3.good()){
                                c = file_list3.get();
                                if(c=='\n') numLines3++;
                            }

                            if(numLines1<7 || numLines2<7 || numLines3<7){
                                cout << "You need more matching points" << endl;
                            }
                            else{

                                cout << "Exit point selection mode" << endl;
                                cout << "Enter transfert mode" << endl;

                                kn::loadMatrix(list1,"input/list1.list");
                                kn::loadMatrix(list2,"input/list2.list");
                                kn::loadMatrix(list3,"input/list3.list");

                                nb_corresp = min(min(list1.rows(), list2.rows()), list3.rows());
                                matching_points = 1;

                                Eigen::Vector3d point1;
                                Eigen::Vector3d point2;
                                Eigen::Vector3d point3;

                                /* Matrix A */
                                Eigen::MatrixXd A = MatrixXd::Zero(4*nb_corresp, 27);

                                /* Fill matrix A */
                                
                                for(int i=0; i<=1; ++i){
                                    for(int l=0; l<=1 ; ++l){
                                        for(int p=0; p<nb_corresp; ++p){
                                            for(int k=0; k<=2; ++k){

                                            point1 << list1(p,0), list1(p,1), list1(p,2);
                                            point2 << list2(p,0), list2(p,1), list2(p,2);
                                            point3 << list3(p,0), list3(p,1), list3(p,2);

                                            A(4*p + 2*i + l, 9*k + 3*i + l) = -point1(k)*point2(2)*point3(2);
                                            A(4*p + 2*i + l, 9*k + 3*i + 2) = point1(k)*point2(2)*point3(l);
                                            A(4*p + 2*i + l, 9*k + 6 + l) = point1(k)*point2(i)*point3(2);
                                            A(4*p + 2*i + l, 9*k + 8) = -point1(k)*point2(i)*point3(l);

                                            // EQ += point1(k)*(point2(i)*point3(2)*T(2,l,k) - point2(2)*point3(2)*T(i,l,k) - point2(i)*point3(l)*T(2,2,k) + point2(2)*point3(l)*T(i,2,k));

                                            }
                                        }
                                    }
                                }


                                /*----- SVD -----*/

                                Eigen::JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
                                //cout << "Singular values : " << endl << svd.singularValues() << endl;
                                //cout << "U Matrix : " << endl << svd.matrixU() << endl;
                                Eigen::MatrixXd V = svd.matrixV();
                                Eigen::VectorXd t = V.col(26);

                                T.fillWith(t);
                            }
                        }
                    }

					if(e.type == SDL_QUIT) {
						loop = 0;
						break;
					}
				}
			}

			/* Quit SDL */

			//SDL_FreeSurface(image1); 
			SDL_FreeSurface(image2); 
			SDL_FreeSurface(image3); 
			IMG_Quit();
			SDL_Quit();

			return EXIT_SUCCESS;
		}
	}
	return 0;
}

