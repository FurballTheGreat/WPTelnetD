using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Windows.Storage;
using ICSharpCode.SharpZipLib.Silverlight.Core;
using ICSharpCode.SharpZipLib.Silverlight.Zip;

namespace AppRepurposer.Common
{
    public static class ZipHelper
    {
        public static async Task UnzipFromStorage(StorageFile pSource, StorageFolder pDestinationFolder, IEnumerable<string> pIgnore)
        {
            using (var stream = await pSource.OpenStreamForReadAsync())
            {
                await UnzipFromStream(stream, pDestinationFolder,pIgnore.ToList());
            }
        }

        public static async Task UnzipFromStream(Stream pZipStream,StorageFolder pDestinationFolder, List<string> pIgnore )
        {

            var zipInputStream = new ZipInputStream(pZipStream);
            var zipEntry = zipInputStream.GetNextEntry();
            while (zipEntry != null)
            {
               var entryFileName = (string)zipEntry.Name;
               
                byte[] buffer = new byte[4096]; 

                // Manipulate the output filename here as desired.

                var directoryName = Path.GetDirectoryName(entryFileName);
                var fileName = Path.GetFileName(entryFileName);
                if (!pIgnore.Contains(fileName.ToLower()))
                {

                    var outFolder = directoryName == null || directoryName.Length == 0
                        ? pDestinationFolder
                        : await GetOrCreateSubFolder(pDestinationFolder, directoryName);


                    StorageFile outFile = null;
                    try
                    {
                        outFile = await outFolder.GetFileAsync(fileName);
                    }
                    catch (Exception)
                    {

                    }
                    if (outFile == null)
                        outFile = await outFolder.CreateFileAsync(fileName, CreationCollisionOption.ReplaceExisting);

                    using (var outStream = await outFile.OpenStreamForWriteAsync())
                    {
                        StreamUtils.Copy(zipInputStream, outStream, buffer);
                    }
                }
                zipEntry = zipInputStream.GetNextEntry();
            }
        }

        private static async Task<StorageFolder> GetOrCreateSubFolder(StorageFolder pOutFolder, string pDirectoryName)
        {
            try
            {
                var folder = await pOutFolder.GetFolderAsync(pDirectoryName);
                if (folder != null)
                    return folder;
            }
            catch (Exception)
            {
                
            }
            return await pOutFolder.CreateFolderAsync(pDirectoryName, CreationCollisionOption.ReplaceExisting);
        }
    }
}

