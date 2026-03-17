#include "eeg_data_manager.h"

EegDataManager::EegDataManager(read_signal_file::SignalFile* signalFile, read_signal_file *signalReader){
    m_signal = signalFile;
    m_signalReader = signalReader;

    int numChannels = m_signal->recorder_info.numberOfChannelsUsed;
    m_rawCache.resize(numChannels);

    // Initialize cache bounds to invalid values so it forces a load on the first draw
    m_cachedStartPage = -1;
    m_cachedEndPage = -1;

}

void EegDataManager::ensureDataLoaded(int requested_start_page, int requested_end_page) { // by Gemini
    // If our cache already holds the requested pages, do nothing! Instant UI response.
    if (requested_start_page >= m_cachedStartPage && requested_end_page <= m_cachedEndPage) {
        return;
    }

    // We don't have the data. We need to load it.
    // OPTIMIZATION: Load a little bit extra (e.g., +1 page on each side)
    // so if the user pans left/right slightly, it's already in RAM.
    int load_start = std::max(0, requested_start_page - 1);
    int load_end = std::min(int(m_signal->signal_pages.size()) - 1, requested_end_page + 1);

    // 1. Calculate how many samples total we need in our buffer
    int total_samples_needed = (load_end - load_start + 1) * m_signal->recorder_info.channels[0].save_buffer_size;

    // 2. Resize m_rawCache if necessary (Zero-allocation after the first big view)
    for(auto& channel_buffer : m_rawCache) {
        if(channel_buffer.size() < total_samples_needed) {
            channel_buffer.resize(total_samples_needed);
        }
    }

    // 3. Read the pages from disk into m_rawCache using your hyper-fast
    // read_signal_page_into() function from Cuculus!
    // Base offset where data starts in the SIG file (after the main file header)
    long base_file_offset = m_signal->data_table.signal_info.offset;
    int page_size_bytes = m_signal->data_table.signal_info.size; // size of one full page in bytes

    int current_sample_start = 0;

    for (int p = load_start; p <= load_end; ++p) {

        // Calculate the physical byte location of Page 'p' in the hard drive
        long specific_page_offset = base_file_offset + (p * page_size_bytes);

        // Read that page and insert it into m_rawCache starting at current_sample_start
        m_signalReader->read_signal_page_into_cache(
                    specific_page_offset,
                    m_signal->recorder_info.numberOfChannelsUsed,
                    m_signal->recorder_info.channels,
                    current_sample_start,
                    m_rawCache
                    );

        // Move the memory pointer forward for the next page
        // Assuming channels have the same sampling rate for simplicity here
        current_sample_start += m_signal->recorder_info.channels[0].save_buffer_size;
    }

    // 4. Update state
    m_cachedStartPage = load_start;
    m_cachedEndPage = load_end;
}

std::vector<std::vector<double> > EegDataManager::applyMontage(const std::vector<std::vector<double> > &raw_segment)
{
    return raw_segment;
}

void EegDataManager::applyFiltersInPlace(std::vector<std::vector<double> > &traces)
{
}

std::vector<std::vector<double>> EegDataManager::getProcessedData(double start_time_sec, double end_time_sec) {

    // 1. Convert requested times to Page indices
    int start_page = start_time_sec / 10;
    int end_page = end_time_sec / 10;

    // 2. CACHE LAYER: Guarantee the raw data is in memory
    ensureDataLoaded(start_page, end_page);

    // 3. Extract just the exact requested time window from the cache
    // (Because the cache might hold 5s to 25s, but the UI only wants 10s to 20s)
    std::vector<std::vector<double>> raw_segment = extractSegment(start_time_sec, end_time_sec);

    // 4. MONTAGE LAYER: Convert Raw Channels to Display Traces
    std::vector<std::vector<double>> traces = applyMontage(raw_segment);

    // 5. FILTER LAYER: Apply fidlib Notch/Bandpass directly to the traces
    applyFiltersInPlace(traces);

    // Return exactly what the UI needs to draw
    return traces; // after implementation of montages and filtering
}

std::vector<std::vector<double>> EegDataManager::extractSegment(double start_time_sec, double end_time_sec) {

    // The trick here is translating "Time in seconds" to a "Local Index" inside our m_rawCache vector.

    // 1) Global Index: Which sample is this in the entire file?
    //    global_sample = time_in_seconds * sampling_rate

    // 2) Cache Start Index: Which global sample does our m_rawCache[0] represent?
    //    cache_start_sample = m_cachedStartPage * samples_per_page

    // 3) Local Index: Where is this in our vector?
    //    local_index = global_sample - cache_start_sample


    // Create the output container
    int num_channels = m_rawCache.size();
    std::vector<std::vector<double>> segment(num_channels);

    // Assume all channels share the same sampling rate for this example.
    // (If they differ, you'd calculate this inside the channel loop).
    int sampling_rate = m_signal->recorder_info.channels[0].sampling_rate;
    int samples_per_page = m_signal->recorder_info.channels[0].save_buffer_size;



    // 1. Calculate global sample indices for the requested time window
    long global_start_sample = static_cast<long>(start_time_sec * sampling_rate);
    long global_end_sample = static_cast<long>(end_time_sec * sampling_rate);

    // 2. Calculate the global sample index where our current cache begins
    long global_cache_start_sample = m_cachedStartPage * samples_per_page;

    // 3. Translate global indices to local vector indices
    long local_start_index = global_start_sample - global_cache_start_sample;
    long local_end_index = global_end_sample - global_cache_start_sample;

    // 4. Safety bounds checking (in case the UI asks for time past the end of the file)
    if (local_start_index < 0) local_start_index = 0;

    long max_cache_index = m_rawCache[0].size() - 1;
    if (local_end_index > max_cache_index) local_end_index = max_cache_index;

    // Calculate how many samples we are actually extracting
    long segment_length = local_end_index - local_start_index + 1;

    // 5. Copy the exact slice out of the cache for each channel
    for (int i = 0; i < num_channels; ++i) {
        // Pre-allocate the exact size to avoid reallocation overhead during copy
        segment[i].resize(segment_length);

        // Fast memory copy using std::copy
        std::copy(
                    m_rawCache[i].begin() + local_start_index,
                    m_rawCache[i].begin() + local_end_index + 1, // +1 because std::copy end is exclusive
                    segment[i].begin()
                    );
    }

    return segment;
}
