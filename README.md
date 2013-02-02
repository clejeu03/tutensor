requirement :
- SDL : sudo apt-get install libsdl1.2-dev
- SDL-image : sudo apt-get install libsdl-image1.2-dev
- SDL-gfx : sudo apt-get install libsdl-gfx1.2-dev 
- eigen : http://eigen.tuxfamily.org/
	* extract the archive (at /home/[user]/ for example)
	* rename "eigen-eigen-5097c01bcdc4/" to "eigen/"

files specifications :
- image format : jpg
- list format : list
- place the image at the root of the application

how to run after the make:

	Notes :
	*	the order of your pictures is important, the processing will be launch on the third one
	* the order of your lists is important too, please respect the order you have made for the image files

	Samples :
	./bin/trifocal image1.jpg image2.jpg image3.jpg 
		* only display your pictures so you can do manually the selection of related points and launch the processing
		* the processing will be launch on the image3.jpg

	./bin/trifocal image1.jpg image2.jpg image3.jpg list1.list list2.list
		* if you have already two lists of points and want to calcul the last one 
		* work with 1 list too
		* please note that you can add as many points as you want from the first two pictures

	./bin/trifocal image1.jpg image2.jpg image3.jpg list1.list list2.list list3.list
		* the application only display the final vision of all corresponding points in the three pictures



