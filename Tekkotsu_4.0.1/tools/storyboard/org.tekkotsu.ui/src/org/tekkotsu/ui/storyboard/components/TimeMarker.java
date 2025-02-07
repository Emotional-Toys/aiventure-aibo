package org.tekkotsu.ui.storyboard.components;

import java.util.Vector;

/**
 * 
 * @author asangpet
 */
public class TimeMarker {
    int defaultGap, xoffset;

    float defaultScale;

    IntVector timeVec;

    IntVector offsetVec;

    class IntVector {
        Vector v;

        public IntVector() {
            v = new Vector();
        }

        public int get(int index) {
            return ((Integer) (v.get(index))).intValue();
        }

        /**
         * @param value
         * @return index of value, or (-insert_index) if not found
         */
        public int seek(int value) {
            int l = 0, r = v.size();
            int mid = (l + r) / 2;
            while (l < r) {
                int test = get(mid);
                if (test < value)
                    l = mid + 1;
                else if (test > value)
                    r = mid;
                else
                    return mid;
                mid = (l + r) / 2;
            }
            if (mid < v.size()) {
                int test = get(mid);
                if (test < value)
                    return -(mid + 1);
                else if (test > value)
                    return -mid;
                else
                    return mid;
            } else
                return -mid;
        }

        public void add(int idx, int value) {
            v.add(idx, new Integer(value));
        }

        public boolean add(int value) {
            return v.add(new Integer(value));
        }

        public int size() {
            return v.size();
        }
    }

    public TimeMarker(int xoff, int gap, float scale) {
        xoffset = xoff;
        defaultGap = gap;
        defaultScale = scale;
        clearMarker();
    }
    
    public void clearMarker() {
        timeVec = new IntVector();
        offsetVec = new IntVector();
        timeVec.add(0);
        offsetVec.add(xoffset);
    }
    
    public synchronized void addMarker(int time, int offset) {
        if (timeVec.get(timeVec.size()-1) != time) {
            timeVec.add(time);
            offsetVec.add(offset);
        }
    }

    public synchronized int getOffset(int time) {
        int idx = timeVec.seek(time);
        // exact match
        if (idx >= 0)
            return offsetVec.get(idx);

        idx = -idx;

        if (idx >= timeVec.size()) {
            //only left boundary available
            int ltime = timeVec.get(timeVec.size() - 1);
            int lbound = offsetVec.get(timeVec.size() - 1);
            int offset = (int) Math.round((time - ltime) * defaultScale
                    * defaultGap)
                    + lbound;            
            return offset;
        } else {
            //right boundary available
            int rtime = timeVec.get(idx);
            int rbound = offsetVec.get(idx);
            int ltime = timeVec.get(idx - 1);
            int lbound = offsetVec.get(idx - 1);
            int offset = (int) Math.round(1.0f * (time - ltime)
                    * (rbound - lbound) / (rtime - ltime)) + lbound;            
            return offset;
        }
    }

    public int getTime(int xoffset) {
        int idx = offsetVec.seek(xoffset);
        // exact match
        if (idx >= 0)
            return timeVec.get(idx);

        idx = -idx;
        if (idx >= offsetVec.size()) {
            //only left boundary available
            int ltime = timeVec.get(offsetVec.size() - 1);
            int lbound = offsetVec.get(offsetVec.size() - 1);
            int time = (int) Math.round((xoffset - lbound) / defaultScale
                    / defaultGap) + ltime;
            //System.out.println("l:"+time);
            return time;
        } else {
            //right boundary available
            int rtime = timeVec.get(idx);
            int rbound = offsetVec.get(idx);
            int ltime = timeVec.get(idx - 1);
            int lbound = offsetVec.get(idx - 1);
            int time = (int) Math.round(1.0f * (xoffset - lbound)
                    * (rtime - ltime) / (rbound - lbound))+ltime;
            //System.out.println("lr:"+time);
            return time;
        }
    }
}
