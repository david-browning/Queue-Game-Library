using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation.Metadata;
using Windows.Storage;

namespace XCopyLauncher
{
    class Program
    {
        static void Main(string[] args)
        {
            string parameters =
                ApplicationData.Current.LocalSettings.Values["params"] as string;

            Console.WriteLine(parameters);

            var splitArgs = parameters.Split(new char[] { ' ' });

            foreach (string s in splitArgs)
            {
                Console.WriteLine(s);
            }

            string destFile = System.IO.Path.Combine(splitArgs[1], "TEST.qgl");
            Console.WriteLine(destFile);

            //Console.WriteLine("xcopy params are:");


            //Process xcopyProc = new Process();
            //xcopyProc.StartInfo.FileName = "xcopy";
            //xcopyProc.StartInfo.Arguments = parameters;
            ////xcopyProc.StartInfo.CreateNoWindow = true;
            ////xcopyProc.StartInfo.RedirectStandardOutput = true;
            ////xcopyProc.StartInfo.UseShellExecute = false;
            //xcopyProc.Start();
            //xcopyProc.WaitForExit();
            //Console.WriteLine("XCopy exited with " + xcopyProc.ExitCode);
            Console.ReadLine();

            return;
        }
    }
}
