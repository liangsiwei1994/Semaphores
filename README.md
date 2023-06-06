<a name="readme-top"></a>
# Semaphores

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#running-the-program">Running the Program</a></li>
      </ul>
    </li>
    <li><a href="#sample-output">Sample Output</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>


<!-- ABOUT THE PROJECT -->
## About The Project

This program is set up to simulate producers-consumers consumer problem that is capable of:
1. Handle any number of producers and consumers
2. Handle any queue size
3. Handle any number of jobs produced
4. Error handling
5. Consumer ending it's thread after having no jobs to consume for 20 seconds.


<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

All you need is a compiler that is able to compile c++ program. :)

### Running the program

1. Clone the repo
   ```sh
   git clone https://github.com/liangsiwei1994/SudokuSolver.git
   ```
2. Open the terminal. Go to the folder/directory path and compile the program using the following command
   ```js
   make
   ```
8. In the same path, run the program using
   ```js
   ./main <queue size> <# of jobs produced per producer> <# of producers> <# of consumers>
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- SAMPLE OUTPUT -->
## Sample Output

The following image shows the expected output when running the program using a queue size of 5, number of jobs produced per producer to be 6, number of producers to be 2 and number of consumers to be 3.

![sampleoutput-screenshot][sampleoutput-screenshot]

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

SiWei - liang_siwei@hotmail.com

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Use this space to list resources you find helpful and would like to give credit to. I've included a few of my favorites to kick things off!

* [Dr. Anandha Gopalan](https://www.imperial.ac.uk/people/a.gopalan)
* [vi-dev0](https://github.com/othneildrew/Best-README-Template.git)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
[sampleoutput-screenshot]: images/SampleOutput.png
