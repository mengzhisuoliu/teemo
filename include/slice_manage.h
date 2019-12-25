/*******************************************************************************
* Copyright (C) 2018 - 2020, winsoft666, <winsoft666@outlook.com>.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
*
* Expect bugs
*
* Please use and enjoy. Please let me know of any bugs/improvements
* that you have found/implemented and I will fix/incorporate them into this
* file.
*******************************************************************************/

#ifndef EFD_SLICE_MANAGE_H__
#define EFD_SLICE_MANAGE_H__
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <future>
#include <condition_variable>
#include "slice.h"
#include "curl/curl.h"
#include "easy_file_download.h"

namespace easy_file_download {
class Slice;
class SliceManage : public std::enable_shared_from_this<SliceManage> {
public:
  SliceManage();
  virtual ~SliceManage();

  Result SetNetworkConnectionTimeout(size_t conn_timeout_ms);
  size_t GetNetworkConnectionTimeout() const;

  Result SetNetworkReadTimeout(size_t read_timeout_ms);
  size_t GetNetworkReadTimeout() const;

  void SetSliceCacheExpiredTime(int seconds);
  int GetSliceCacheExpiredTime() const;

  Result SetThreadNum(size_t thread_num);
  size_t GetThreadNum() const;

  void SetEnableSaveSliceFileToTempDir(bool enabled);
  bool IsEnableSaveSliceFileToTempDir() const;

  void SetMaxDownloadSpeed(size_t byte_per_seconds);
  size_t GetMaxDownloadSpeed() const;

  Result Start(const std::string &url, const std::string &target_file_path,
               ProgressFunctor progress_functor, RealtimeSpeedFunctor realtime_speed_functor);
  void Stop();

  std::string GetUrl() const;
  std::string GetTargetFilePath() const;
  std::string GetIndexFilePath() const;
  std::string DumpSlicesInfo() const;

protected:
  long QueryFileSize() const;
  bool LoadSlices(const std::string url, ProgressFunctor functor);
  bool CombineSlice();
  bool CleanupTmpFiles();
  bool UpdateIndexFile();
  void Destory();
  std::string GenerateIndexFilePath(const std::string &target_file_path) const;

protected:
  std::string url_;
  std::string target_file_path_;
  std::string index_file_path_;
  bool enable_save_slice_to_tmp_dir_;
  size_t thread_num_;
  size_t network_conn_timeout_;
  size_t network_read_timeout_;
  size_t max_download_speed_;
  int slice_cache_expired_seconds_;
  long file_size_;
  CURLM *multi_;
  ProgressFunctor progress_functor_;
  RealtimeSpeedFunctor speed_functor_;
  std::vector<std::shared_ptr<Slice>> slices_;
  std::future<void> progress_notify_thread_;
  std::future<void> speed_notify_thread_;

  bool stop_;
  std::mutex stop_mutex_;
  std::condition_variable stop_cond_var_;
};
} // namespace easy_file_download

#endif // !EFD_SLICE_MANAGE_H__