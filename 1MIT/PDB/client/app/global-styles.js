import { injectGlobal } from 'styled-components';

/* eslint no-unused-expressions: 0 */
injectGlobal`
  html,
  body {
    height: 100%;
    width: 100%;
  }

  body, .ms-Fabric {
    font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;
  }

  body.fontLoaded, body.fontLoaded .ms-Fabric {
    font-family: 'Open Sans', sans-serif;
  }

  #app {
    min-height: 100%;
    height: 100%;
    min-width: 100%;
  }
  
  a:hover {
    text-decoration: none;
  }
  
  button:focus {
    outline: none;
  }
  
  .ms-Callout {
    max-width: 300px;
  }
  .ms-CalloutCoverExample-buttonArea {
    vertical-align: top;
    display: inline-block;
    min-width: 130px;
    margin: 0 100px;
    text-align: center;
    height: 32px;
}
`;

