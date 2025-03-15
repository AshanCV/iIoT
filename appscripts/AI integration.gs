function callAI() {
  // Define sheet names and ranges
  var dashboardSheetName = "Dashboard";
  var dataSheetName = "Sheet1";
  
  // Open the spreadsheet
  var spreadsheet = SpreadsheetApp.getActiveSpreadsheet();
  var dashboardSheet = spreadsheet.getSheetByName(dashboardSheetName);
  var dataSheet = spreadsheet.getSheetByName(dataSheetName);

  // Get user prompt from Dashboard sheet (e.g., cell A1)
  var userPrompt = dashboardSheet.getRange("G10").getValue();

  // Get data from Sheet1 for context
  var dataRange = dataSheet.getDataRange();
  var data = dataRange.getValues();
  var dataContext = data.slice(1).map(row => row.join(", ")).join("\n"); // Skip header row

  // Combine user prompt with data context
  var fullPrompt = "Analyze the following data and respond to the prompt:\n" + dataContext + "\n\nPrompt: " + userPrompt;

  // Call OpenAI API
  var apiKey = "         "; // Replace with your OpenAI API key
//  var apiUrl = "https://api.openai.com/v1/completions";
  
  var options = {
    method: "post",
    headers: {
      "Content-Type": "application/json",
      "Authorization": "Bearer " + apiKey
    },
    payload: JSON.stringify({
      model: "gpt-3.5-turbo", // Use GPT-3.5 or GPT-4
     messages: [
        {
          role: "user",
          content: fullPrompt
        }
      ],
      max_tokens: 150, // Adjust based on response length
      temperature: 0.7 // Adjust for creativity (0 = deterministic, 1 = creative)
    }),
    muteHttpExceptions: true // Add this to see full error messages
  };

 // Fetch response from OpenAI API
  var response = UrlFetchApp.fetch(apiUrl, options);
  var jsonResponse = JSON.parse(response.getContentText());
  
  // Log the full response for debugging
  Logger.log(jsonResponse);

  // Check for errors
  if (jsonResponse.error) {
    Logger.log("Error: " + jsonResponse.error.message);
    return;
  }

  // Extract AI response
  var aiResponse = jsonResponse.choices[0].message.content.trim();

  // Display AI response in Dashboard sheet (e.g., cell B1)
  dashboardSheet.getRange("G12").setValue(aiResponse);
}
