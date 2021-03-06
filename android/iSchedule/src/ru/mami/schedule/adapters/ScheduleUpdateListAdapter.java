package ru.mami.schedule.adapters;

import java.util.ArrayList;

import ru.mami.schedule.R;
import ru.mami.schedule.utils.Subject;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;


public class ScheduleUpdateListAdapter extends BaseAdapter {
    private LayoutInflater inflater;
    private ArrayList<Subject> subjects;


    public ScheduleUpdateListAdapter(Context context, ArrayList<Subject> subjects) {
        inflater = LayoutInflater.from(context);
        this.subjects = subjects;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        if (convertView == null) {
            convertView = inflater.inflate(R.layout.check_item, null);
        }

        Subject subj = subjects.get(position);

        TextView subjView  = (TextView) convertView.findViewById(R.id.check_subject);
        TextView startDateView  = (TextView) convertView.findViewById(R.id.check_start_date);
        TextView endDateView  = (TextView) convertView.findViewById(R.id.check_end_date);
        TextView dowView  = (TextView) convertView.findViewById(R.id.check_dow);
        TextView timeView  = (TextView) convertView.findViewById(R.id.check_time);
        TextView groupView  = (TextView) convertView.findViewById(R.id.check_group);
        TextView placeView  = (TextView) convertView.findViewById(R.id.check_place);

        subjView.setText(subj.getSubject());
        startDateView.setText(subj.getDt_start());
        endDateView.setText(subj.getDt_end());
        dowView.setText(subj.getDow());

        String dt_start = subj.getT_start();
        dt_start = new StringBuffer(dt_start).insert(2, ".").toString();
        String dt_end = subj.getT_end();
        dt_end = new StringBuffer(dt_end).insert(2, ".").toString();
        timeView.setText(dt_start + ":" + dt_end);

        groupView.setText(subj.getGroups());
        placeView.setText(subj.getPlace());

        return convertView;
    }

    @Override
    public int getCount() {
         return subjects.size();
    }

    @Override
    public Object getItem(int position) {
        return position;
    }

    @Override
    public long getItemId(int position) {
        return position;
    }
}
