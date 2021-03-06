package ru.mami.schedule.activities;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;
import ru.mami.schedule.R;
import ru.mami.schedule.adapters.UserAdapter;

import java.util.HashMap;
import java.util.Map;

public class UserInfoDialogActivity extends Activity implements OnClickListener{
    private Map<String, String> user = new HashMap<String, String>();

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.info_layout);

        Context context = getApplicationContext();
        UserAdapter userAdapter = new UserAdapter(context);
        user = userAdapter.getUserAsMap();

    }

    @Override
    protected void onStart() {
        super.onStart();
        TextView name = (TextView) findViewById(R.id.nameInfo);
        TextView login = (TextView) findViewById(R.id.loginInfo);
        TextView phone = (TextView) findViewById(R.id.phoneInfo);
        TextView email = (TextView) findViewById(R.id.emailInfo);

        name.setText(user.get("name"));
        login.setText(user.get("login"));
        phone.setText(user.get("phone"));
        email.setText(user.get("email"));
    }

    @Override
    public void onClick(View arg0) {
        finish();
    }
}
