using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TimeLineTool
{
    //public class TempDataType : ITimeLineDataItem
    //{
    //    public DateTime? StartTime { get; set; }
    //    public DateTime? EndTime { get; set; }
    //    public Boolean TimelineViewExpanded { get; set; }
    //    public String Name { get; set; }
    //}
    public class TempDataType 
    {
        public int StartTime { get; set; }
        public int EndTime { get; set; }
        public int Duration { get; set; }
        public Boolean TimelineViewExpanded { get; set; }
        public String Name { get; set; }

        //private String startTimeInfo;
        //private String endTimeInfo;
        //private String durationTimeInfo;

        //public String StartTimeInfo
        //{
        //    get { string l_str = "S:"; l_str += StartTime.ToString(); return l_str; }
        //}
        //public String EndTimeInfo
        //{
        //    get { endTimeInfo = "E:"; endTimeInfo += EndTime.ToString(); return endTimeInfo; }
        //}
        //public String DurationTimeInfo
        //{
        //    get { endTimeInfo = "Duration:"; durationTimeInfo += (EndTime - StartTime).ToString(); return durationTimeInfo; }
        //}
    }
}
