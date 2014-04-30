package ru.romanov.schedule.src;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ListView;
import ru.romanov.schedule.R;
import ru.romanov.schedule.adapters.ScheduleUpdateListAdapter;
import ru.romanov.schedule.adapters.SubjectAdapter;
import ru.romanov.schedule.utils.Subject;

import java.util.ArrayList;

public class UpdateListActivity extends Activity implements OnClickListener {

    @Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.check_list_activity_layout);
	}

	@Override
	protected void onStart() {
		super.onStart();

        SubjectAdapter adapter = new SubjectAdapter(this);
        ArrayList<Subject> subjects =  adapter.getNewSubjects();

        ScheduleUpdateListAdapter updater = new ScheduleUpdateListAdapter(this, subjects);
        ListView l = (ListView) findViewById(R.id.checkList);
        l.setAdapter(updater);

		
		Button confirmButton = (Button) findViewById(R.id.check_confirm_button);
		confirmButton.setOnClickListener(this);

	}

	@Override
	public void onClick(View v) {
		switch (v.getId()){
		case R.id.check_confirm_button:

			break;
		}
	}
}
