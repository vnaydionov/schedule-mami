package ru.romanov.schedule.src;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.protocol.HTTP;
import ru.romanov.schedule.R;
import ru.romanov.schedule.adapters.UserAdapter;
import ru.romanov.schedule.utils.RequestStringsCreater;
import ru.romanov.schedule.utils.StringConstants;
import ru.romanov.schedule.utils.UpdateManager;
import ru.romanov.schedule.utils.XMLParser;

import javax.xml.parsers.ParserConfigurationException;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Map;

public class IScheduleActivity extends Activity {

	private SharedPreferences mSharedPreferences;
	private EditText loginEditText;
	private EditText passEditText;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mSharedPreferences = getSharedPreferences(
				StringConstants.SCHEDULE_SHARED_PREFERENCES, MODE_PRIVATE);
		
		if (mSharedPreferences.getString(StringConstants.TOKEN, null) == null) {
			setContentView(R.layout.main);
		} else {
			setUpdateAlarm();
			startMainTabActivity();
		}
	}

	@Override
	protected void onStart() {
		this.loginEditText = (EditText) findViewById(R.id.loginEText);
		this.passEditText = (EditText) findViewById(R.id.passEText);
		Button loginButton = (Button) findViewById(R.id.logInButton);
		loginButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				PostRequestAuthManager pram = new PostRequestAuthManager(
						loginEditText.getText().toString(), passEditText
								.getText().toString());
				pram.execute();

			}
		});
		super.onStart();
	}

	@Override
	protected void onResume() {
		super.onResume();
	}
	
	private void setUpdateAlarm() {
		AlarmManager alarm = (AlarmManager) getSystemService(Context.ALARM_SERVICE);
		int alarmType = AlarmManager.ELAPSED_REALTIME_WAKEUP;
		Intent intent = new Intent(this, UpdateManager.class);
		PendingIntent pendingIntent = PendingIntent.getBroadcast(this, 0, intent, 0);
		alarm.setRepeating(alarmType, 
				10000, 1*60*1000, pendingIntent);
	}

	private void startMainTabActivity() {
		Intent intent = new Intent(this, MainTabActivity.class);
		startActivity(intent);
		finish();
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// TODO Auto-generated method stub
		super.onCreateOptionsMenu(menu);
		menu.add(Menu.NONE, R.id.menu_settings, Menu.NONE, "Настройки");
		//MenuInflater inflater = getMenuInflater();
		//inflater.inflate(R.menu.main_menu, menu);
		return true;
	}
	
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Intent intent;
		switch (item.getItemId()) {
		case R.id.menu_settings:
			intent = new Intent(this, MenuSettingsActivity.class);
			startActivity(intent);
			break;
		default:
			break;
		}

		return true;
	}

	
	@SuppressWarnings("unused")
	private class PostRequestAuthManager extends
			AsyncTask<Void, Void, HttpResponse> {

		private String login;
		private String pass;
		private String token;
		private ProgressDialog dialog;
		
		public PostRequestAuthManager(String login, String pass) {
			this.login = login;
			this.pass = pass;
		}

		@Override
		protected void onPreExecute() {
			super.onPreExecute();
			dialog = ProgressDialog.show(IScheduleActivity.this, "", getString(R.string.loading), true);
		}
		@Override
		protected HttpResponse doInBackground(Void... params) {
			HttpClient client = new DefaultHttpClient();
			String reqString = null;
			try {
				reqString = RequestStringsCreater.createAuthRequestSting(
						login, pass);
			} catch (ParserConfigurationException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			HttpResponse responce = null;
			try {
				String url = mSharedPreferences.getString("host", "");
				String port = mSharedPreferences.getString("port", "");
				String uri = url + ":" + port + "/main";
				HttpPost request = new HttpPost(uri);
				
				StringEntity entity = new StringEntity(reqString, "UTF-8");
				request.setHeader(HTTP.CONTENT_TYPE, "text/xml");
				request.setEntity(entity);
				responce = client.execute(request);

			} catch (ClientProtocolException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
			return responce;
		}

		@Override
		protected void onPostExecute(HttpResponse result) {
			// TODO Auto-generated method stub
			super.onPostExecute(result);
			if (result != null) {
				HttpEntity ent = result.getEntity();
				try {
					BufferedReader r = new BufferedReader(
							new InputStreamReader(ent.getContent()));
					StringBuilder total = new StringBuilder();
					String line;
					while ((line = r.readLine()) != null) {
						total.append(line);
					}
					Map<String, String> resultMap = XMLParser.parseResponse(total.toString(), "/response/*");
					if(resultMap.get(XMLParser.STATUS).equals(XMLParser.OK)) {
                        resultMap = XMLParser.parseResponse(total.toString(), "/response/login-session/*");
						Toast.makeText(IScheduleActivity.this,
								getString(R.string.auth_success), Toast.LENGTH_LONG).show();
						this.token = resultMap.get(XMLParser.TOKEN);
						saveSessionData();
						
						resultMap = XMLParser.parseResponse(total.toString(), "/response/login-session/user/*");
						Context context = getApplicationContext();
						UserAdapter us = new UserAdapter(context);
						us.saveUser(resultMap.get(XMLParser.NAME), resultMap.get(XMLParser.LOGIN), 
								resultMap.get(XMLParser.EMAIL), resultMap.get(XMLParser.PHONE));
						setUpdateAlarm();
						startMainTabActivity();
						
					} else {
						Toast.makeText(IScheduleActivity.this,
								getString(R.string.login_error), Toast.LENGTH_LONG).show();
					}
				} catch (IllegalStateException e) {
					e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				} catch (Exception e) {
					e.printStackTrace();
				}
			} else {
				Toast.makeText(IScheduleActivity.this, "Не получилось соединиться с серверoм.", Toast.LENGTH_LONG).show();
			}
			dialog.dismiss();
		}
		
		private void saveSessionData() {
			Editor editor = IScheduleActivity.this.mSharedPreferences.edit();
			editor.putString(StringConstants.TOKEN, this.token);
			editor.commit();
		}

	}
}
