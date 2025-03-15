function doGet(e) {
  Logger.log(JSON.stringify(e));
  var result = "OK";
  
  var sheet_id = "1wIr0E_tS1qazy3WV7LnIqfw1oyLrv3TDneTxtFUs0ZA";
  var sheet_name = "Sheet1";

   var sheet = SpreadsheetApp.openById(sheet_id).getSheetByName(sheet_name);
  var lastRow = sheet.getLastRow();  // Get last row number

  // Writing Data (ESP32 → Google Sheets)
  if (e.parameter.sts == "write") {
    var inputs = [
      e.parameter.in1, e.parameter.in2, e.parameter.in3,
      e.parameter.in4, e.parameter.in5, e.parameter.in6
    ];
    var outputs = [
      e.parameter.o1, e.parameter.o2, e.parameter.o3,
      e.parameter.o4, e.parameter.o5, e.parameter.o6
    ];

    var timestamp = new Date();

    // Insert new data below the header row (pushing older records downward)
    sheet.insertRowAfter(1);
    sheet.getRange(2, 1, 1, 13).setValues([[timestamp, ...inputs, ...outputs]]);

    return ContentService.createTextOutput("Data Logged Successfully!");
  }

  // Reading Manual Command from Fixed Cell (e.g., Cell B1)
  if (e.parameter.sts == "read") {
    var commandO1 = sheet.getRange("O1").getValue();  // Read command from O1
    var commandP1 = sheet.getRange("P1").getValue();  // Read command from P1
    
    var commandResponse = commandO1 + "," + commandP1;  // Combine commands
  
    return ContentService.createTextOutput(commandResponse); 
  }

  return ContentService.createTextOutput("Invalid Request");
}

