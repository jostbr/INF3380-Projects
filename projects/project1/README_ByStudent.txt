
-----------------------------------------------------------------------------------
File best viewed without word-wrap because of manual line changes.
-----------------------------------------------------------------------------------

By student: Jostein Brændshøi
Comments clarifying various practical matters in my answer to the assignment:
-----------------------------------------------------------------------------------

-----------------------------------------------------------------------------------
* The parallel program parallel_main.c is written to run on any number of
  multiple processes, and while testing with from 2 to 50 procs I found that
  2 always gave the fastest execution time. See parallel_main_time.jpg. This
  seems reasonable given that I run Ubuntu in a virtual machine (VirtualBox)
  and have allocated 2 (of my 4) CPU cores to the virtual operating system.
  When running on more than 2, these may be virtually simulated within the two
  cores and we would have to perform more communication operations (which consumes
  time) without actually having any more hardware power. Based on observation the
  timing results found in timing_results.txt are generated when running parallel
  _main.con 2 processes.
-----------------------------------------------------------------------------------


-----------------------------------------------------------------------------------
* When running the parallel program I couldn't use the specified way:

    ---$ ./parallel_main iters kappa input_jpeg_filename output_jpeg_filename

  because it didn't let me use more than one MPI-process this way. Therefore
  I had to use the following way to run the program (inside parallel folder):

    ---$ mpirun -n <num_procs> ./parallel_main -np <num_procs> iters, kappa,
         input_jpeg_filename, output_jpeg_filename

  By using this method I gained access to num_procs MPI-processes. And when
  doing this we get the following relations between the command-line arguments
  and elements of the argv array:

    argv[0] = ./parallel_main           // Program name.
    argv[1] = -np                       // MPI argument.
    argv[2] = num_procs                 // MPI argument.
    argv[3] = iters                     // 1st "actual" argument.
    argv[4] = kappa                     // 2nd "actual" argument.
    argv[5] = input_jpeg_filename       // 3rd "actual" argument.
    argv[6] = output_jpeg_filename      // 4th "actual" argument.

  See start of main-function in parallel_main for implementation of the above.
-----------------------------------------------------------------------------------


-----------------------------------------------------------------------------------
* Because of trouble importing the import_export_jpeg.h header file using
    
    # include <../simple-jpeg/import_export_jpeg.h>

  on my operating system, I had to use the following include-line:

    # include </home/cyclone/INF3380/ME01/simple-jpeg/import_export_jpeg.h>

  which can be found on line 6 in both serial_main.c and parallel_main.c.
-----------------------------------------------------------------------------------


-----------------------------------------------------------------------------------
* The test denoised images MonaLisa_DENOISED_SERIAL.jpg and MonaLisa_DENOISED_
  PARALLEL.jpg in the serial and parallel folder respectively, are generated
  using iters = 45 and kappa = 0.1. In addition, in the parallel folder, there
  is an image MonaLisa_PARTIONING_ILLUSTRATION.jpg illustrating the partioning
  of the image among num_procs processes. For creating this illutration, num_procs
  = 8 was used.
-----------------------------------------------------------------------------------


-----------------------------------------------------------------------------------
* Because of having confused myself a lot mixing matrix dimensions with symbols
  I choose to use image_height and image_width instead of m and n respectively,
  just to keep tings clearer while working and not mixing dimensions. This
  change (relative to the template) can be found in both programs. Also in
  parallel_main my_m and my_n are substitued with my_height and my_width
  respectively.
-----------------------------------------------------------------------------------


-----------------------------------------------------------------------------------
* At the end of the parallel template it's suggested that all processes sends
  its u_bar to process 0 and then process 0 copies them appropriately into
  whole_image. At last process 0 would convert whole_image to jpeg (image_chars).
  The method that suited my setup better works (and is implemented) as follows:
  After iso diffusion, each process resulting u_bar is converted to my_image_chars
  and then all these num_procs my_image_chars are combined (using MPI_Gather())
  and sent to process 0 where it's stored in the image_chars array. Finally
  process zero exports the combined image_chars to JPEG and creates the image.
-----------------------------------------------------------------------------------
