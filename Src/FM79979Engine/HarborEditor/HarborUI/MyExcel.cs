using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//using ExcelLibrary.CompoundDocumentFormat;
//using ExcelLibrary.SpreadSheet;

//
using Excel = Microsoft.Office.Interop.Excel;
using System.Data;


namespace HarborUI
{
    public class MyExcel
    {

        public MyExcel()
        {
            //ExcelTest();
        }
        static public void ReadExcelTest()
        {
            //	using namespace System::Windows::Forms::Integration;
            //==================================
            //read xls or xlsm file
            //==================================
            Excel.Application excelApp = new Excel.Application();
            String filename = "C:/Users/USER/Desktop/3939889.xls";
            Microsoft.Office.Interop.Excel.Workbook wb = excelApp.Workbooks.Open(filename);
            Microsoft.Office.Interop.Excel.Worksheet  exWs  = (Microsoft.Office.Interop.Excel.Worksheet)excelApp.ActiveSheet;
            int row=1;
            int col=1;
            String tmp=((Microsoft.Office.Interop.Excel.Range)exWs.Cells[(System.Object)row, (System.Object)col]).Value2.ToString();
            int a = 0;
        }

        static public String ExcelToXML(String e_strExcelFileName)
        {
            String  l_strXMLFileName = System.IO.Path.ChangeExtension(e_strExcelFileName,"xml");
            String l_sss;
            try
            {
                System.Data.OleDb.OleDbConnection MyConnection;
                System.Data.DataSet ds;
                System.Data.OleDb.OleDbDataAdapter MyCommand;


                String l_strExtension = System.IO.Path.GetExtension(e_strExcelFileName);

                String l_strCombineFileName = "";
                if (l_strExtension == ".xls" || l_strExtension == ".XLS")
                {
                    l_strCombineFileName = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + e_strExcelFileName + ";Extended Properties=\"Excel 8.0;HDR=Yes;IMEX=2\"";

                }
                else if (l_strExtension == ".xlsx" || l_strExtension == ".XLSX")
                {
                    l_strCombineFileName = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + e_strExcelFileName + ";Extended Properties=\"Excel 12.0;HDR=Yes;IMEX=2\"";
                }
                //String l_strCombineFileName = "provider=Microsoft.Jet.OLEDB.4.0;Data Source='Specify path here ";
                //l_strCombineFileName += e_strExcelFileName;
                //l_strCombineFileName += "';Extended Properties=Excel 8.0;";
                //"provider=Microsoft.Jet.OLEDB.4.0;Data Source='Specify path here \\xl2xml.xls';Extended Properties=Excel 8.0;"
                MyConnection = new System.Data.OleDb.OleDbConnection(l_strCombineFileName);
                //set sheet name here default is sheet1
                try
                {
                    //MyCommand = new System.Data.OleDb.OleDbDataAdapter("select * from [Sheet1$]", MyConnection);
                    //lazy for this,because ExportDataSetToExcel has 3 unuse sheet
                    MyCommand = new System.Data.OleDb.OleDbDataAdapter("select * from [Sheet4$]", MyConnection);
                    //MyCommand.TableMappings.Add("Table", "Product");
                    ds = new System.Data.DataSet("RouteData");
                    MyCommand.Fill(ds);
                    //from element to attribute.
                    foreach (DataTable dt in ds.Tables)
                    {
                        foreach (DataColumn dc in dt.Columns)
                        {
                            dc.ColumnMapping = MappingType.Attribute;
                        }
                    }
                    //String l_sss = ds.GetXmlSchema();
                    l_sss = ds.GetXml();
                    MyConnection.Close();
                    ds.WriteXml(l_strXMLFileName);
                }
                catch (System.Exception e)
                {
                    l_sss = e.ToString();
                }
                return l_sss;
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.ToString());
            }
            return "failed";
        }

        static public void ExcelTest()
        {
            //Create an Emplyee DataTable
            DataTable employeeTable = new DataTable("Employee");
            employeeTable.Columns.Add("Employee ID");
            employeeTable.Columns.Add("Employee Name");
            employeeTable.Rows.Add("1", "ABC");
            employeeTable.Rows.Add("2", "DEF");
            employeeTable.Rows.Add("3", "PQR");
            employeeTable.Rows.Add("4", "XYZ");

            //Create a Department Table
            DataTable departmentTable = new DataTable("Department");
            departmentTable.Columns.Add("Departme 6nt ID");
            departmentTable.Columns.Add("Department Name");
            departmentTable.Columns.Add("test");
            //departmentTable.Rows.Add("Department ID", "Department Name");
            departmentTable.Rows.Add("88", "IT", "999");
            departmentTable.Rows.Add("99", "HR", "999");
            departmentTable.Rows.Add("300", "Finance", "999");

            //Create a DataSet with the existing DataTables
            DataSet ds = new DataSet("Organization");
            ds.Tables.Add(employeeTable);
            ds.Tables.Add(departmentTable);
            Save("C:/Users/USER/Desktop/123456789.xlsx",ds);
            //Save("333.xls", ds);
        }

        public static void Save(String e_strFileName,DataSet e_ds)
        {
            if ( System.IO.File.Exists(e_strFileName) == true )
            {
                //System.IO.File.Create(e_strFileName);
                System.IO.File.Delete(e_strFileName);
            }
            String l_strFileName = System.IO.Path.GetFileName(e_strFileName);
            string docPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            docPath += "/";
            docPath += l_strFileName;
            //fuck change DataRowExtensions name
            ExportDataSetToExcel(e_ds, l_strFileName);
            if (System.IO.File.Exists(docPath) == true)
            {
                System.IO.File.Copy(docPath, e_strFileName);
                System.IO.File.Delete(docPath);
            }
        }
        //https://support.office.com/zh-tw/article/Excel-%E4%B8%AD%E6%94%AF%E6%8F%B4%E7%9A%84%E6%AA%94%E6%A1%88%E6%A0%BC%E5%BC%8F-a28ae1d3-6d19-4180-9209-5a950d51b719?ui=zh-TW&rs=zh-TW&ad=TW
        static private void ExportDataSetToExcel(DataSet ds, string e_strFileName)
        {
            string docPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            docPath += "/";
            docPath += e_strFileName;
            if (System.IO.File.Exists(docPath) == true)
            {
                System.IO.File.Delete(docPath);
            }
            //Creae an Excel application instance
            Excel.Application excelApp = new Excel.Application();
            //show app
            //excelApp.Visible = true;
            if (excelApp == null)
            {
                System.Windows.Forms.MessageBox.Show("Excel is not properly installed!!");
                return;
            }
            excelApp.DisplayAlerts = false;
            object misValue = System.Reflection.Missing.Value;
            //Create an Excel workbook instance and open it from the predefined location
            //Excel.Workbook excelWorkBook = excelApp.Workbooks.Add(misValue);// Open(e_strFileName);
            Excel.Workbook excelWorkBook = excelApp.Workbooks.Add();
            //Excel.Workbook excelWorkBook = excelApp.Workbooks.Open(e_strFileName);
            //fuck...crash have no idea...
            //for (int i = 0; i < excelWorkBook.Worksheets.Count; i++)
            //{

            //    Excel.Worksheet wkSheet = (Excel.Worksheet)excelWorkBook.Worksheets.get_Item(i);
            //    //wkSheet.Name;
            //    wkSheet.Delete();
            //}
            //while ( excelWorkBook.Worksheets.Count!= 0)
            //{
            //     Worksheet wkSheet = excelWorkBook.Worksheets[i];
            //     wkSheet.Name = 
            //}
            excelApp.DisplayAlerts = true;
            foreach (DataTable table in ds.Tables)
            {
                //Add a new worksheet to workbook with the Datatable name
                Excel.Worksheet excelWorkSheet = excelWorkBook.Sheets.Add();
                try
                {
                    excelWorkSheet.Name = table.TableName;
                }
                catch (System.Exception e)
                {
                    System.Windows.Forms.MessageBox.Show(e.ToString());
                }
                for (int i = 1; i < table.Columns.Count + 1; i++)
                {
                    excelWorkSheet.Cells[1, i] = table.Columns[i - 1].ColumnName;
                }

                for (int j = 0; j < table.Rows.Count; j++)
                {
                    for (int k = 0; k < table.Columns.Count; k++)
                    {
                        excelWorkSheet.Cells[j + 2, k + 1] = table.Rows[j].ItemArray[k].ToString();
                    }
                }
            }
            excelApp.DisplayAlerts = false;
            try
            {
                //excelWorkBook.SaveAs(e_strFileName, Excel.XlFileFormat.xlWorkbookNormal, misValue, misValue, misValue, misValue, Excel.XlSaveAsAccessMode.xlExclusive, misValue, misValue, misValue, misValue, misValue);
                //excelWorkBook.Save();
                //excelWorkBook.SaveAs(e_strFileName,Excel.XlFileFormat.xlWorkbookDefault,
                //Type.Missing, Type.Missing, true, false,
                //Excel.XlSaveAsAccessMode.xlExclusive, Excel.XlSaveConflictResolution.xlLocalSessionChanges, Type.Missing, Type.Missing);
                excelWorkBook.SaveAs(e_strFileName);
           
            }
            catch (Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.ToString());
                //excelWorkBook.Close(true);
            }

//            excelWorkBook.SaveAs(e_strFileName, Microsoft.Office.Interop.Excel.XlFileFormat.xlWorkbookDefault, Type.Missing, Type.Missing, true, false, Excel.XlSaveAsAccessMode.xlNoChange, Excel.XlSaveConflictResolution.xlLocalSessionChanges, Type.Missing, Type.Missing);
            //releaseObject(excelWorkBook);
            //releaseObject(excelApp);
            excelWorkBook.Close();
            releaseObject(excelWorkBook);
            excelApp.Quit();
            releaseObject(excelApp);

        }

        static private void releaseObject(object obj)
        {
            try
            {
                System.Runtime.InteropServices.Marshal.ReleaseComObject(obj);
                obj = null;
            }
            catch (Exception ex)
            {
                obj = null;
                System.Windows.Forms.MessageBox.Show("Exception Occured while releasing object " + ex.ToString());
            }
            finally
            {
                GC.Collect();
            }
        }
    }
}
