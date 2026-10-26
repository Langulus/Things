///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Thing.hpp>
#include <Langulus/Part.hpp>


namespace Langulus::Things
{
   struct File;
   struct Folder;

   ///                                                                        
   ///   Abstract file system module                                          
   struct FileSystem : virtual Module {
      using CTTI_Bases = Module;

   protected:
      // Working directory path.                                        
      // This is the only full path that is exposed to the system,      
      // unless LANGULUS(PARANOID) is enabled.                          
      Path mWorkingPath;
      // Main data directory, for both reading and writing.             
      // Relative to mWorkingPath.                                      
      Path mMainDataPath;

   public:
      virtual auto GetFile  (Path const&) const -> Ref<Things::File>   = 0;
      virtual auto GetFolder(Path const&) const -> Ref<Things::Folder> = 0;

      auto GetWorkingPath() const noexcept -> Path const&;
      auto GetDataPath()    const noexcept -> Path const&;
   };


   ///                                                                        
   ///   Abstract file interface                                              
   struct File : virtual Part {
      using CTTI_Bases = Part;
      using CTTI_Producer = FileSystem;

   protected:
      Path   mFilePath;
      bool   mExists = false;
      DMeta  mFormat;
      size_t mByteCount = 0;
      bool   mIsReadOnly = false;

      // Parent directory substring, mapped onto mFilePath              
      Token mParentDirectory;
      // File name after all directories, mapped onto mFilePath         
      Token mFileName;
      // The extension of the filename, mapped onto mFilePath           
      Token mFileExtension;

   public:
      virtual auto ReadAs        (DMeta)       const -> Many = 0;
      virtual auto RelativeFile  (Path const&) const -> Ref<Things::File> = 0;
      virtual auto RelativeFolder(Path const&) const -> Ref<Things::Folder> = 0;
      
      auto Exists()      const noexcept -> bool;
      auto IsReadOnly()  const noexcept -> bool;
      auto GetFilePath() const noexcept -> Path const&;
      auto GetFormat()   const noexcept -> DMeta;
      auto GetBytesize() const noexcept -> size_t;

      template<class T>
      T ReadAs() const;


      ///                                                                     
      /// Abstract file reader stream                                         
      struct Reader {
      protected:
         Ref<File const> mFile;
         size_t    mProgress = 0;

      public:
         Reader() = delete;
         virtual ~Reader() {}

         Reader(File const* f) : mFile {f} {}

         virtual auto Read(Bytes&) -> size_t = 0;

         auto& GetFile() const noexcept { return mFile; }
      };


      ///                                                                     
      /// Abstract file writer stream                                         
      struct Writer {
      protected:
         Ref<File> mFile;
         size_t    mProgress = 0;
         bool      mAppend = false;

      public:
         Writer() = delete;
         virtual ~Writer() {}

         Writer(File* f, bool append)
            : mFile   {f}
            , mAppend {append} {}

         virtual auto Write(Bytes const&) -> size_t = 0;

         auto& GetFile() const noexcept { return mFile; }
      };

      virtual auto NewReader() const      -> Ref<Reader> = 0;
      virtual auto NewWriter(bool append) -> Ref<Writer> = 0;
   };


   ///                                                                        
   ///   Abstract folder interface                                            
   struct Folder : virtual Part {
      using CTTI_Bases = Part;
      using CTTI_Producer = FileSystem;

   protected:
      Path mFolderPath;
      bool mExists = false;
      bool mIsReadOnly = false;

   public:
      virtual auto RelativeFile  (Path const&) const -> Ref<Things::File> = 0;
      virtual auto RelativeFolder(Path const&) const -> Ref<Things::Folder> = 0;

      auto Exists()        const noexcept -> bool;
      auto IsReadOnly()    const noexcept -> bool;
      auto GetFolderPath() const noexcept -> Path const&;
   };
}

#include "IO.inl"