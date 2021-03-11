/*
    @author = ngx123
    @brief = global defines for pmm
*/


/*
	@breif = initialises the PMM(physical memory manager)
	@return = 0 on success, -1 on fail
*/
extern int initPMM(void);

/*
	@brief = allocates specified amount of continues frames and returns there start address
	@param frame_count = amount of continous frames that should be allocated
	@return = address of the first frame out of the allocated ones, NULL on error
*/
void* palloc(size_t frame_count);

/*
	@brief = frees specified amount of frames starting at index of the specified start address of a frame
	@param frame_address = from which frame to start freeing
	@param frame_count = amount of frames to free
	@return = 0 on success, -1 on fail
*/
int pfree(address_tt frame_address, size_t frame_count);

/*
	@brief = reserves specified amount of frames starting at index of the specified address
	@param frame_address = address of frame from which reserving should start
	@param frame_count = amount of frames to reserve
	@return = 0 on success , -1 on fail
*/
int reserveRAM(address_tt frame_address, size_t frame_count);

/*
	@brief = unreserved specified amount of frames starting at index of the specified address
	@param frame_address = address of frame from which unreserving should start
	@param frame_count = amount of frames to unreserve
	@return = 0 on success , -1 on fail
*/
int unreserveRAM(address_tt frame_address, size_t frame_count);