#ifndef EEG_DATA_MANAGER_H
#define EEG_DATA_MANAGER_H

#include <vector>
#include <string>
#include "read_signal_file.h"

// Define how a single trace on the screen is calculated
// e.g., for Bipolar "Fp1 - F3", pos_ch_index is Fp1, neg_ch_index is F3.
struct MontageTrace {
    std::string trace_name;
    int pos_ch_index;
    int neg_ch_index; // -1 if Common Reference
};

class EegDataManager {
public:
    EegDataManager(read_signal_file::SignalFile* signalFile);

    // The main API for the UI
    // Returns a 2D vector where [trace_index][sample_index]
    std::vector<std::vector<double>> getProcessedData(double start_time_sec, double end_time_sec);

    // Configuration APIs
    void setMontage(const std::vector<MontageTrace>& montage);
    void setFilters(bool enableNotch, double highPass, double lowPass);
    std::vector<std::vector<double> > extractSegment(double start_time_sec, double end_time_sec);

private:
    read_signal_file::SignalFile* m_signal;
    std::vector<MontageTrace> m_currentMontage;

    // --- CACHE STATE ---
    int m_cachedStartPage;
    int m_cachedEndPage;
    // Raw data buffer: [channel_index][sample_index]
    std::vector<std::vector<double>> m_rawCache;

    // --- INTERNAL PIPELINE STEPS ---
    void ensureDataLoaded(int start_page, int end_page);
    std::vector<std::vector<double>> applyMontage(const std::vector<std::vector<double>>& raw_segment);
    void applyFiltersInPlace(std::vector<std::vector<double>>& traces);
};

#endif // EEG_DATA_MANAGER_H
