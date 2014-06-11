package ru.romanov.schedule.adapters;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.TextView;
import ru.romanov.schedule.R;
import ru.romanov.schedule.utils.Subject;

import java.util.ArrayList;

public class SubjectsListAdapter extends BaseExpandableListAdapter{
	
	private ArrayList<ArrayList<String>> groups;
    private ArrayList<ArrayList<Subject>> children;
    private Context context;
    
    public SubjectsListAdapter(Context context, ArrayList<ArrayList<String>> groups, ArrayList<ArrayList<Subject>> children) {
    	this.context = context;
    	this.children = children;
    	this.groups = groups;
    }

	@Override
	public Subject getChild(int groupPosition, int childPosition) {
		return children.get(groupPosition).get(childPosition);
	}

	@Override
	public long getChildId(int groupPosition, int childPosition) {
		return childPosition;
	}

	@Override
	public View getChildView(int groupPosition, int childPosition, boolean isLastChild, View convertView,
			ViewGroup parent) {
		Subject subj = getChild(groupPosition, childPosition);
		
		if (convertView == null) {
            LayoutInflater infalInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = infalInflater.inflate(R.layout.schedule_item_element, null);
        }
		
		TextView subjView = (TextView) convertView.findViewById(R.id.schedule_item_subject);
		TextView groupView = (TextView) convertView.findViewById(R.id.schedule_item_group);
		TextView timeView = (TextView) convertView.findViewById(R.id.schedule_item_time);
		TextView placeView = (TextView) convertView.findViewById(R.id.schedule_item_place);
		TextView actView = (TextView) convertView.findViewById(R.id.schedule_item_act);

        String time_start = subj.getT_start();
        String time_end = subj.getT_end();
        time_start = new StringBuffer(time_start).insert(2, ".").toString();
        time_end = new StringBuffer(time_end).insert(2, ".").toString();

		subjView.setText(subj.getSubject());
		groupView.setText(subj.getGroups());
		timeView.setText(time_start + ":" + time_end);
		placeView.setText(subj.getPlace());
		actView.setText(subj.getActivities());

		return convertView;
	}

	@Override
	public int getChildrenCount(int groupPosition) {
		return children.get(groupPosition).size();
	}

	@Override
	public ArrayList<String> getGroup(int groupPosition) {
		return groups.get(groupPosition);
	}

	@Override
	public int getGroupCount() {
		return groups.size();
	}

	@Override
	public long getGroupId(int groupPosition) {
		return groupPosition;
	}

	@Override
	public View getGroupView(int groupPosition, boolean isExpanded, View convertView, ViewGroup parent) {
		ArrayList<String> group = getGroup(groupPosition);
		
		if (convertView == null) {
            LayoutInflater infalInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = infalInflater.inflate(R.layout.schedule_list_item, null);
        }
		
		TextView dateView = (TextView) convertView.findViewById(R.id.schedule_list_item_date);
        TextView dowView = (TextView) convertView.findViewById(R.id.schedule_list_item_weekday);
		
		dateView.setText(group.get(0));
        dowView.setText(group.get(1));

        if (isExpanded) {
            dateView.setTextColor(Color.parseColor("#DF004F"));
            dowView.setTextColor(Color.parseColor("#DF004F"));
        }
        else {
            dateView.setTextColor(Color.BLACK);
            dowView.setTextColor(Color.BLACK);
        }

		
		return convertView;
	}

	@Override
	public boolean hasStableIds() {
		return true;
	}

	@Override
	public boolean isChildSelectable(int arg0, int arg1) {
		return true;
	}

}
